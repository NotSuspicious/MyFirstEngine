// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

// GLOBAL VARIABlES
const std::string vertexShaderPath = "./Build/MyFirstEngine/shader.vert";
const std::string fragShaderPath = "./Build/MyFirstEngine/shader.frag";

float vertices[] = {
        0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};
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

int main(int argc, char * argv[]) {

    GLFWwindow* m_Window = CreateWindow("OpenGL", mWidth, mHeight);
    CreateOpenGLContext(m_Window);


    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(m_Window);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

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
