#include "Game.h"
#include <fstream>

Game::Game()
    : m_isRunning(true)
{
    m_Instance = this;
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

GLuint GenerateTexture(const char* image, int& width, int& height, int& nChannels);
bool Game::Initialize()
{
    m_Window = CreateWindow("OpenGL", mWidth, mHeight);
    CreateOpenGLContext(m_Window);

    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwSetCursorPosCallback(m_Window, mouse_callback);
    glfwSetScrollCallback(m_Window, scroll_callback);
}
