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

    int m_iLayerIdx; // 오브젝트가 소속되어있는 Layer 의 Index

    bool m_bDead;

public:
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    void AddComponent(CComponent* _Component);
    CComponent* GetComponent(COMPONENT_TYPE _Type) const { return m_arrCom[(UINT)_Type]; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);

    CGameObject* GetParent() const { return m_Parent; }
    void DisconnectWithParent();
    void DisconnectWithLayer();

    void AddChild(CGameObject* _Child);
    bool IsDead() const { return m_bDead; }

public:
    const vector<CGameObject*>& GetChildObject() const { return m_vecChild; }

    GET_COMPONENT(Light3D, LIGHT3D);

public:
    CGameObject();
    virtual ~CGameObject();

    friend class CLayer;
    friend class CTaskMgr;
};
