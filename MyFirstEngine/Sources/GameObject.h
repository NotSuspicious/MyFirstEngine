#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "Component.h"
#include "component.h"

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

    const glm::vec3& GetPosition() const { return m_Position; }
    const glm::quat& GetRotation() const { return m_Rotation; }
    const glm::vec3& GetScale() const { return m_Scale; }
    
private: 

    std::vector<Component*> m_Components;
    
    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
    State m_State;

    void Update() const
    {
        if (m_State != ACTIVE)
            return;
        for(Component* component : m_Components)
        {
            component->Update(0.0f);
        }
    }

    void ProcessInput() const
    {
        if (m_State != ACTIVE)
            return;
        for(Component* component : m_Components)
        {
            component->ProcessInput();
            OnProcessInput();
        }
    }

    void OnProcessInput() const
    {
        // Do something
    }
};