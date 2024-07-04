#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "Component.h"

class GameObject
{
public: 
    enum State {
        ACTIVE,
        PAUSED,
        DESTROY
    };
    
    GameObject(const glm::vec3& position, glm::quat& rotation, glm::vec3& scale)
        :m_Position(position), m_Rotation(rotation), m_Scale(scale)
    { }

    GameObject()
        :m_Position(glm::vec3(0.0f)),
        m_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
        m_Scale(glm::vec3(1.0f))
    { }

    void AddComponent(Component* const component) 
    {
        m_Components.push_back(component);
    }

    glm::vec3& GetPosition() { return m_Position; }
    glm::quat& GetRotation() { return m_Rotation; }
    glm::vec3& GetScale() { return m_Scale; }
    
public: 

    std::vector<Component*> m_Components;
    
    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
    State m_State;

    void Update(const float deltaTime) const
    {
        if (m_State != ACTIVE)
            return;
        for(Component* component : m_Components)
        {
            component->Update(deltaTime);
        }
    }

    void ProcessInput(GLFWwindow* window) const
    {
        if (m_State != ACTIVE)
            return;
        for(Component* component : m_Components)
        {
            component->ProcessInput(window);
            OnProcessInput();
        }
    }

    void OnProcessInput() const
    {
        // Do something
    }
};