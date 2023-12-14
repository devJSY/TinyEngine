#pragma once
#include "CEntity.h"

class CGameObject;

class CComponent : public CEntity
{
private:
    const COMPONENT_TYPE m_Type;
    CGameObject* m_Owner;

public:
    virtual void begin() {}
    virtual void tick() {}
    virtual void finaltick() = 0;
    virtual void UpdateData(){};

public:
    COMPONENT_TYPE GetType() const { return m_Type; }
    CGameObject* GetOwner() const { return m_Owner; }

public:
    CComponent(COMPONENT_TYPE _Type);
    ~CComponent();

    friend class CGameObject;
};
