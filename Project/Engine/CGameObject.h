#pragma once
#include "CEntity.h"

#define GET_COMPONENT(Type, TYPE)                                                                                      \
    class C##Type* Type()                                                                                              \
    {                                                                                                                  \
        return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE];                                                       \
    }

class CComponent;
class CRenderComponent;

class CScript;
class CCamera;

class CGameObject : public CEntity
{
private:
    CComponent* m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent* m_RenderCom;

    vector<CScript*> m_vecScript;
    vector<CGameObject*> m_vecChild;

    CGameObject* m_Parent;

public:
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    void AddComponent(CComponent* _Component);
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);

    CGameObject* GetParent() { return m_Parent; }
    void DisconnectWithParent();
    void AddChild(CGameObject* _Child);

public:
    CGameObject();
    virtual ~CGameObject();
};
