#pragma once

class GameObject;

class Component
{
public:
    Component(GameObject* owner)
        :m_Owner(owner)
    {

    }

    virtual ~Component()
    {
        //m_Owner->RemoveComponent(this);
    }

    GameObject* GetOwner() const { return m_Owner; }

    virtual void Update(float deltaTime) = 0;
    virtual void ProcessInput(class GLFWwindow* window) = 0;
    
protected:
    GameObject* m_Owner;

public: 
    GameObject* GetOwner() { return m_Owner; }
};