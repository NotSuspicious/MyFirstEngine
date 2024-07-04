#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "GameObject.h"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
class Camera : public Component
{
public:
    Camera(GameObject* owner, int width, int height) : Component(owner)
    {
        UpdateProjection(width, height);
    }
    ~Camera() = default;

    void Update(float deltaTime) override
    {
        m_speed = 0.025f * deltaTime;
        UpdateView();
    }

    void ProcessInput(GLFWwindow* window) override
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_Owner->GetPosition() += m_speed * m_cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_Owner->GetPosition() -= m_speed * m_cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_Owner->GetPosition() -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_Owner->GetPosition() += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * m_speed;
    }

    void UpdateView()
    {
        glm::vec3 direction;
        float pitch = m_pitch;
        float yaw = m_yaw;
        direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        direction.y = glm::sin(glm::radians(pitch));
        direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        m_cameraFront = glm::normalize(direction);

        m_view = glm::lookAt(m_Owner->GetPosition(), m_Owner->GetPosition() + m_cameraFront, m_cameraUp);
    }

    void UpdateProjection(int width, int height)
    {
        m_projection = glm::perspective(glm::radians(m_fov), (float)width / (float)height, 0.1f, 100.0f);
    }

    glm::mat4 GetView() const { return m_view; }
    glm::mat4 GetProjection() const { return m_projection; }

    void mouse_callback(GLFWwindow* window, double xoffset, double yoffset) {
        xoffset *= m_sensitivity;
        yoffset *= m_sensitivity;
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
    
public:
    glm::vec3 m_target;
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    const float m_sensitivity = 0.1f;
    
    
    float m_pitch = 0.0f;
    float m_yaw = -90.0f;
    float m_speed = 0.1f;
    float m_fov = 45.0f;    

    glm::mat4 m_view;
    glm::mat4 m_projection;
 
   
};