#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Component.h"
#include "GameObject.h"
#include "GLFW/glfw3.h"
class Camera : public Component
{
public:
    void Update(float deltaTime) override
    {
        
    }

    void ProcessInput() override
    {
        if (glfwGetKey(m_Owner->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            m_cameraPos += m_speed * m_cameraFront;
        if (glfwGetKey(m_Owner->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            m_cameraPos -= m_speed * m_cameraFront;
        if (glfwGetKey(m_Owner->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_speed;
        if (glfwGetKey(m_Owner->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_speed;
    }

    
private:
    glm::vec3 m_Target;
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    float m_pitch = 0.0f;
    float m_yaw = -90.0f;
    float m_speed = 0.1f;
    float m_fov = 45.0f;    

    void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos; 
        lastY = ypos;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        m_yaw += xoffset;
        m_pitch += yoffset;
        if (m_pitch > 89.0f) {
            m_pitch = 89.0f;
        }
        if (m_pitch < -89.0f) {
            m_pitch = -89.0f;
        }
    }

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    m_fov -= (float)yoffset;
        if (m_fov < 1.0f) 
            m_fov = 1.0f;
        if (m_fov > 60.0f)
            m_fov = 60.0f;
    }
   
};