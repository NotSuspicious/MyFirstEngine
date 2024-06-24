// System Headers
#include "Component.h"
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
}

GLuint shaderProgram;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
float cameraSpeed;


const float sensitivity = 0.1f;

float fov = 45.0f;

//GLOBAL VARIABLES

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

void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char * argv[]) {


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
        projection = glm::perspective(glm::radians(fov), (float)mWidth/(float)mHeight, 0.1f, 100.0f);
        
        glm::vec3 direction;
        direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        direction.y = glm::sin(glm::radians(pitch));
        direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        m_cameraFront = glm::normalize(direction);

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
