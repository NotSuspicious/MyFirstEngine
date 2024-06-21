// System Headers
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

// GLOBAL VARIABlES
const std::string vertexShaderPath = "./Shaders/shader.vert";
const std::string fragShaderPath = "./Shaders/shader.frag";

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

const int mWidth = 1200;
const int mHeight = 800;

const char* williamImg = "./Assets/william.jpg";
const char* faceImg = "./Assets/awesomeface.png";

const unsigned int vertexSize = sizeof(float) * 5;

int textureCount = 0;

GLuint shaderProgram;

glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
float cameraSpeed;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
//GLOBAL VARIABLES

const char* LoadShaderAsString(const std::string &filename) {
    std::string result;
    std::string line;

    std::ifstream file(filename);

    if(file.is_open()) {
        while (std::getline(file, line)) {
            result += line + "\n";
        }
        file.close();
    } else {
        printf("Failed to open file: %s\n", filename.c_str());
    }
    char* cstr = new char[result.length() + 1];
    strcpy(cstr, result.c_str());
    return cstr;
}


GLuint InitializeShaders() {
    const char* vertexSource = LoadShaderAsString(vertexShaderPath);
    const char* fragSource = LoadShaderAsString(fragShaderPath);
    //Create and Load a Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    //Create and Load Frag Shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);

    //Vertext Shader Compilation Debugging
    GLint vertexStatus, fragStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexStatus);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragStatus);
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    if (vertexStatus && fragStatus) {
        printf("Shaders compiled successfully!");
    } else {
        printf("Shader compilation failed!");
    }

    //Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);

    //Specify what buffer Frag Shader output is written
    glBindFragDataLocation(shaderProgram, 0, "FragColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    return shaderProgram;
}

GLFWwindow* CreateWindow(const char* title, int width, int height) {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return NULL;
    }

    return mWindow;
}

void CreateOpenGLContext(GLFWwindow* mWindow) {
    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
}

GLuint GenerateTextureFromImage(const char* image, int& width, int& height, int& nChannels) {
    if (textureCount > 31) {
        fprintf(stderr, "Too many textures\n");
        return -1;
    }

    //Create Texture Buffer Object
    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + textureCount);
    glBindTexture(GL_TEXTURE_2D, tex);

    //Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //Set Mipmap Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //load image
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load(
        image, &width, &height, &nChannels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load texture\n");
        return -1;
    }   

    if (nChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (nChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        fprintf(stderr, "Unsupported number of channels: %d\n", nChannels);
        return -1;
    }
    //Generate Mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    textureCount++;
    return tex;
}

void OffsetVertices(float offset) {
    GLuint offsetIndex = glGetUniformLocation(shaderProgram, "m_yVertexOffset");
    float offsetRef;
    glGetUniformfv(shaderProgram, offsetIndex, &offsetRef);
    glUniform1f(offsetIndex, offsetRef + offset);
}

void OffsetCamera(glm::vec3 offset) {
    GLfloat glMatrix[16];
    GLuint viewUni = glGetUniformLocation(shaderProgram, "view");
    glGetUniformfv(shaderProgram, viewUni, glMatrix);
    glm::mat4 view = glm::make_mat4(glMatrix);
    view = glm::translate(view, glm::vec3(0.0f, 5.0f, 0.0f));
    glUniformMatrix4fv(viewUni, 1, GL_FALSE, glm::value_ptr(view));
}

void OnUpArrowPressed() {
    cameraPos += cameraSpeed * m_cameraFront;
}

void OnDownArrowPressed() {
    cameraPos -= cameraSpeed * m_cameraFront;
}

void OnLeftArrowPressed() {
    cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
}

void OnRightArrowPressed() {
    cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
}

void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        OnUpArrowPressed();

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        OnDownArrowPressed();

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        OnLeftArrowPressed();

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        OnRightArrowPressed();
}


int main(int argc, char * argv[]) {

    GLFWwindow* m_Window = CreateWindow("OpenGL", mWidth, mHeight);
    CreateOpenGLContext(m_Window);

    GLuint elements[] = {
        0, 1, 2,
        4, 5, 1,
        3, 2, 6,
        4, 5, 6,
        2, 3, 0,
        1, 0, 4, 
        6, 7, 3, 
        6, 7, 4,
        1, 5, 6, 
        6, 2, 1,
        4, 0, 3, 
        3, 7, 4
    };

    //Vertex Array Object VAO: Stores attributes and VBO links
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Creates a Vertex Buffer Object (VBO)
    GLuint vbo;
    glGenBuffers(1, &vbo);
    //Activates the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //Copy Vertex Data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Create & Initialize Element Buffer Object
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(elements), elements, GL_STATIC_DRAW);

    shaderProgram = InitializeShaders();

    // GLint colAttribute = glGetAttribLocation(shaderProgram, "color");
    // glEnableVertexAttribArray(colAttribute);
    // glVertexAttribPointer(colAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*)(sizeof(float)*3));

    //Get reference to postion from vertex shader
    GLint posAttribute = glGetAttribLocation(shaderProgram, "position");
    GLint texCoordAttribute = glGetAttribLocation(shaderProgram, "texCoord");
    //Enable the attribute
    glEnableVertexAttribArray(posAttribute);
    glEnableVertexAttribArray(texCoordAttribute);
    //Specify how data is retrieved from VBO
    glVertexAttribPointer(posAttribute, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
    glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*)(sizeof(float)*3));

    //Load Textures
    int width1, height1, nChannels1;
    GLuint tex1 = GenerateTextureFromImage(williamImg, width1, height1, nChannels1);
    int width2, height2, nChannels2;
    GLuint tex2 = GenerateTextureFromImage(faceImg, width2, height2, nChannels2);

    //Get uniform reference from frag shader
    GLuint textureUni1 = glGetUniformLocation(shaderProgram, "m_Texture1");
    GLuint textureUni2 = glGetUniformLocation(shaderProgram, "m_Texture2");
    glUniform1i(textureUni1, 0);
    glUniform1i(textureUni2, 1);


    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    

    glEnable(GL_DEPTH_TEST);

    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // Rendering Loop
    while (glfwWindowShouldClose(m_Window) == false) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cameraSpeed = 2.5f * deltaTime;

        ProcessInput(m_Window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));


        // view = glm::rotate(view, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)mWidth/(float)mHeight, 0.1f, 100.0f);
        
        view = glm::lookAt(cameraPos, cameraPos + m_cameraFront, m_cameraUp);

        // glm::mat4 view;
        // view = CameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        // view = CameraTranslate(glm::vec3(camX, 0.0f, camZ));
        
        GLuint modelUni = glGetUniformLocation(shaderProgram, "model");
        GLuint viewUni = glGetUniformLocation(shaderProgram, "view");
        GLuint projectionUni = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(modelUni, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewUni, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionUni, 1, GL_FALSE, glm::value_ptr(projection));

        

        for (size_t i = 0 ; i < sizeof(cubePositions) ; i++) {
            glm::mat4 world = glm::mat4(1.0f);
            world = glm::translate(world, cubePositions[i]);
            GLuint worldUni = glGetUniformLocation(shaderProgram, "world");
            glUniformMatrix4fv(worldUni, 1, GL_FALSE, glm::value_ptr(world));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    
        // glDrawElements(GL_TRIANGLES, sizeof(elements), GL_UNSIGNED_INT, 0);

        // Flip Buffers and Draw
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}
