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
class CCollider2D;
class CAnimator2D;
class Light2D;
class Light3D;
class CTileMap;
class CParticleSystem;
class CSkyBox;

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
    void render(Ptr<CMaterial> _mtrl);

public:
    void AddComponent(CComponent* _Component);
    void RemoveComponent(COMPONENT_TYPE _Type);
    CComponent* GetComponent(COMPONENT_TYPE _Type) const { return m_arrCom[(UINT)_Type]; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(Light3D, LIGHT3D);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
    GET_COMPONENT(SkyBox, SKYBOX);

    CGameObject* GetParent() const { return m_Parent; }
    const vector<CScript*>& GetScripts() const { return m_vecScript; }
    CRenderComponent* GetRenderComponent() const { return m_RenderCom; }

    template <typename T>
    T* GetScript()
    {
        for (size_t i = 0; i < m_vecScript.size(); ++i)
        {
            if (dynamic_cast<T*>(m_vecScript[i]))
                return (T*)m_vecScript[i];
        }
        return nullptr;
    }

    void DisconnectWithParent();
    void DisconnectWithLayer();

    void AddChild(CGameObject* _Child);
    bool IsDead() const { return m_bDead; }

    void Destroy();

     bool IsAncestor(CGameObject* _Other);

public:
    const vector<CGameObject*>& GetChildObject() const { return m_vecChild; }

    int GetLayerIdx() const { return m_iLayerIdx; }

    CLONE(CGameObject);

public:
    CGameObject();
    CGameObject(const CGameObject& origin);
    virtual ~CGameObject();

    friend class CLayer;
    friend class CTaskMgr;
};
