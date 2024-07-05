#pragma once

#include "CEntity.h"

#define GET_COMPONENT(Type, TYPE)                                                                                                                    \
    class C##Type* Type()                                                                                                                            \
    {                                                                                                                                                \
        return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE];                                                                                     \
    }

class CComponent;
class CRenderComponent;

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

    // Bone Socket
    tBoneSocket* m_BoneSocket;

public:
    void begin();
    void tick();
    virtual void finaltick();
    void render();
    void render(Ptr<CMaterial> _mtrl);

public:
    void AddComponent(CComponent* _Component);
    void RemoveComponent(COMPONENT_TYPE _Type);
    void RemoveScript(CScript* _script);
    CComponent* GetComponent(COMPONENT_TYPE _Type) const { return m_arrCom[(UINT)_Type]; }

    template <typename T>
    T* GetComponent()
    {
        for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
        {
            if (dynamic_cast<T*>(m_arrCom[i]))
                return (T*)m_arrCom[i];
        }
        return nullptr;
    }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Animator, ANIMATOR);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(Light, LIGHT);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Rigidbody2D, RIGIDBODY2D);
    GET_COMPONENT(BoxCollider2D, BOXCOLLIDER2D);
    GET_COMPONENT(CircleCollider2D, CIRCLECOLLIDER2D);
    GET_COMPONENT(PolygonCollider2D, POLYGONCOLLIDER2D);
    GET_COMPONENT(EdgeCollider2D, EDGECOLLIDER2D);
    GET_COMPONENT(Rigidbody, RIGIDBODY);
    GET_COMPONENT(BoxCollider, BOXCOLLIDER);
    GET_COMPONENT(SphereCollider, SPHERECOLLIDER);
    GET_COMPONENT(CapsuleCollider, CAPSULECOLLIDER);
    GET_COMPONENT(MeshCollider, MESHCOLLIDER);
    GET_COMPONENT(CharacterController, CHARACTERCONTROLLER);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
    GET_COMPONENT(SkyBox, SKYBOX);
    GET_COMPONENT(Decal, DECAL);
    GET_COMPONENT(LandScape, LANDSCAPE);
    GET_COMPONENT(TextRender, TEXTRENDER);

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

    bool IsAncestor(CGameObject* _Other);

public:
    const vector<CGameObject*>& GetChildObject() const { return m_vecChild; }

    int GetLayerIdx() const { return m_iLayerIdx; }

public:
    const tBoneSocket* GetBoneSocket() const { return m_BoneSocket; }
    void SetBoneSocket(tBoneSocket* _BoneSocket) { m_BoneSocket = _BoneSocket; }

    CLONE(CGameObject);

public:
    CGameObject();
    CGameObject(const CGameObject& origin);
    virtual ~CGameObject();

    friend class CLayer;
    friend class CTaskMgr;
    friend class CLevelSaveLoad;
};
