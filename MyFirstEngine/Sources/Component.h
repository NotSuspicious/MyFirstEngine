#pragma once

class GameObject;

class Component
{
public:
    Component(GameObject* owner)
        :m_Owner(owner)
    {

    }

    virtual ~Component();
    GameObject* GetOwner() const { return m_Owner; }

    virtual void Update(float deltaTime) = 0;
    virtual void ProcessInput() = 0;
    
protected:
    GameObject* m_Owner;
};