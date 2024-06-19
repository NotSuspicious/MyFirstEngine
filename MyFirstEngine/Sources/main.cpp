// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

// GLOBAL VARIABlES
const std::string vertexShaderPath = "./shader.vert";
const std::string fragShaderPath = "./shader.frag";

float vertices[] = {
        0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};

const int mWidth = 1200;
const int mHeight = 800;
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
    glBindFragDataLocation(shaderProgram, 0, "outColor");

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

void GenerateTextures() {
    //     GLuint elements[] = {
    //     0, 1, 2,
    //     2, 3, 0
    // };

    // //Create & Initialize Element Buffer Object
    // GLuint ebo;
    // glGenBuffers(1, &ebo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    //     sizeof(elements), elements, GL_STATIC_DRAW);

    // //Create Texture Buffer Object
    // GLuint tex;
    // glGenTextures(1, &tex);
    // glBindTexture(GL_TEXTURE_2D, tex);

    // //Texture Wrapping
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // // If you use clamp to border
    // // float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    // // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    // //Set Mipmap Filtering
    // glTexParameteri(GL_TEXTURE_2D,
    //     GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D,
    //     GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // //load image
    // int width, height, nrChannels;
    // unsigned char *data = stbi_load(
    //     image, &width, &height, &nrChannels, 0);
    // if (!data) {
    //     fprintf(stderr, "Failed to load texture\n");
    // }   
    // //Generate Texture
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0,GL_RGB, GL_FLOAT, data);
    // //Texture Filtering
    // glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(data);
}

int main(int argc, char * argv[]) {

    GLFWwindow* m_Window = CreateWindow("OpenGL", mWidth, mHeight);
    CreateOpenGLContext(m_Window);

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

    GLuint shaderProgram = InitializeShaders();

    //Get reference to postion from vertex shader
    GLint posAttribute = glGetAttribLocation(shaderProgram, "position");
    GLint colAttribute = glGetAttribLocation(shaderProgram, "color");
    //Enable the attribute
    glEnableVertexAttribArray(posAttribute);
    glEnableVertexAttribArray(colAttribute);
    //Specify how data is retrieved from VBO
    glVertexAttribPointer(posAttribute, 2, GL_FLOAT, GL_FALSE,
        sizeof(float)*5, 0);
    glVertexAttribPointer(colAttribute, 3, GL_FLOAT, GL_FALSE,
        sizeof(float)*5, (void*)(sizeof(float)*2));

    

    // Rendering Loop
    while (glfwWindowShouldClose(m_Window) == false) {
        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_Window, true);



        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
       

        glDrawArrays(GL_TRIANGLES, 0, 3);


        // Flip Buffers and Draw
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}
