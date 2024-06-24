#pragma once

#include "Component.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include <vector>

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

public:
    Game();
    void AddGameObject(class GameObject* gameObject);
    void RemoveGameObject(class GameObject* gameObject);

    const static Game* Instance() { return m_Instance; }

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

    std::vector<class GameObject*> m_GameObjects;
    std::vector<class GameObject*> m_PendingGameObjects;

    GLFWwindow* m_Window;

    GLuint shaderProgram;
    bool m_isRunning;

    void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);
    void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
};