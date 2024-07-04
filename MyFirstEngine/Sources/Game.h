#pragma once

#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include <glad/glad.h>
#include <vector>

class GameObject;

class Game
{
const int mWidth = 1200, mHeight = 800;

const std::string vertexShaderPath = "./Shaders/shader.vert";
const std::string fragShaderPath = "./Shaders/shader.frag";

const char* williamImg = "./Assets/william.jpg";
const char* faceImg = "./Assets/awesomeface.png";
const char* dogImg = "./Assets/dog.png";

const unsigned int vertexSize = sizeof(float) * 5;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = mWidth / 2.0f;
float lastY = mHeight / 2.0f;

int textureCount = 0;

bool firstMouse;

GLuint shaderProgram;   
GameObject* m_CameraObj;
class Camera* m_Camera;

float vertices[180] = {
    //Position                          //Texture Coords
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

std::vector<glm::vec3> cubePositions;

public:
    Game();
    void AddGameObject(GameObject* gameObject);
    void RemoveGameObject(GameObject* gameObject);

    static Game* Instance() { return m_Instance; }

    bool Initialize();
    void Loop();
    void Shutdown();

    bool CreateOpenGLContext(GLFWwindow* mWindow);
    GLFWwindow* CreateWindow(const char* title, int width, int height);
    GLuint InitializeShaders();
    const char* LoadShaderAsString(const std::string& filename);
    GLuint GenerateTexture(const char* image, int& width, int& height, int& nChannels);
private:

    void ProcessInput();
    void UpdateGame();

    static Game* m_Instance;

    std::vector<GameObject*> m_GameObjects;
    std::vector<GameObject*> m_PendingGameObjects;

    GLFWwindow* m_Window;

    bool m_isRunning;

    void Mouse_Callback(GLFWwindow*, double xpos, double ypos);
    void Scroll_Callback(GLFWwindow*, double xoffset, double yoffset);

    static void Mouse_Callback_Static(GLFWwindow*, double xpos, double ypos);
    static void Scroll_Callback_Static(GLFWwindow*, double xoffset, double yoffset);
public:
    bool IsRunning() { return m_isRunning; }
    GLFWwindow* GetWindow() const { return m_Window; }
};