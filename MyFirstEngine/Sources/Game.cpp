#include "glm/fwd.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstdio>
#include <istream>

#include "Camera.h"
#include "GameObject.h"
#include "Game.h"
#include "Component.h"

Game* Game::m_Instance = nullptr;

Game::Game()
    : m_isRunning(true)
{
    m_Instance = this;
}

bool Game::Initialize()
{
    cubePositions = {
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

    m_Window = CreateWindow("OpenGL", mWidth, mHeight);
    CreateOpenGLContext(m_Window);

    //Create a camera gameobject
    m_CameraObj = new GameObject();
    m_Camera = new Camera(m_CameraObj);
    m_CameraObj->AddComponent(m_Camera);
    AddGameObject(m_CameraObj);

    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwSetCursorPosCallback(m_Window, Mouse_Callback_Static);
    glfwSetScrollCallback(m_Window, Scroll_Callback_Static);

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

    // //Create & Initialize Element Buffer Object
    // GLuint ebo;
    // glGenBuffers(1, &ebo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    //     sizeof(elements), elements, GL_STATIC_DRAW);

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
    GLuint tex1 = GenerateTexture(williamImg, width1, height1, nChannels1);
    int width2, height2, nChannels2;
    GLuint tex2 = GenerateTexture(faceImg, width2, height2, nChannels2);

    //Get uniform reference from frag shader
    GLuint textureUni1 = glGetUniformLocation(shaderProgram, "m_Texture1");
    GLuint textureUni2 = glGetUniformLocation(shaderProgram, "m_Texture2");
    glUniform1i(textureUni1, 0);
    glUniform1i(textureUni2, 1);    

    glEnable(GL_DEPTH_TEST);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
}

void Game::Loop()
{
    if (glfwWindowShouldClose(m_Window) == true)
    {
        m_isRunning = false;
    }

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    ProcessInput();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Background Fill Color
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));


    // view = glm::rotate(view, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view;
    glm::vec3 direction;
    float pitch = m_Camera->m_pitch;
    float yaw = m_Camera->m_yaw;
    direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    direction.y = glm::sin(glm::radians(pitch));
    direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    m_Camera->m_cameraFront = glm::normalize(direction);

    glm::vec3 pos = m_Camera->GetOwner()->GetPosition();
    view = glm::lookAt(pos, pos + m_Camera->m_cameraFront, m_Camera->m_cameraUp);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(m_Camera->m_fov), (float)mWidth/(float)mHeight, 0.1f, 100.0f);
    

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

void Game::Shutdown()
{
    glfwTerminate();
}

void Game::AddGameObject(class GameObject *gameObject)
{
    m_PendingGameObjects.push_back(gameObject);
}

void Game::RemoveGameObject(class GameObject *gameObject)
{
    auto iter = std::find(m_GameObjects.begin(), m_GameObjects.end(), gameObject);
    if (iter != m_GameObjects.end())
    {
        m_GameObjects.erase(iter);
    }
    else
    {
        iter = std::find(m_PendingGameObjects.begin(), m_PendingGameObjects.end(), gameObject);
        if (iter != m_PendingGameObjects.end())
        {
            m_PendingGameObjects.erase(iter);
        }
    }
}

bool Game::CreateOpenGLContext(GLFWwindow* mWindow)
{
    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    return true;
}

GLFWwindow* Game::CreateWindow(const char* title, int width, int height)
{
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


GLuint Game::InitializeShaders()
{
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

const char* Game::LoadShaderAsString(const std::string& filename)
{
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

GLuint Game::GenerateTexture(const char* image, int& width, int& height, int& nChannels)
{
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

void Game::Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos; 
    lastY = ypos;
        
    m_Camera->mouse_callback(m_Window, xoffset, yoffset);
}

void Game::Scroll_Callback(GLFWwindow*, double xoffset, double yoffset)
{
    m_Camera->scroll_callback(m_Window, xoffset, yoffset);
}

void Game::Mouse_Callback_Static(GLFWwindow* window, double xpos, double ypos)
{
    Game::Instance()->Mouse_Callback(window, xpos, ypos);
}

void Game::Scroll_Callback_Static(GLFWwindow* window, double xpos, double ypos)
{
    Game::Instance()->Scroll_Callback(window, xpos, ypos);
}

void Game::ProcessInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);
}