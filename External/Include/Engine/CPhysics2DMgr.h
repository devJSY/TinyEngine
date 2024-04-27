#pragma once

#include <box2d\\b2_world_callbacks.h>

// RayCast
struct RaycastHit2D
{
    Vec2 Centroid;              // Raycast 중심 위치
    float Distance;             // Centroid 에서 부터 Ray를 발사하여 충돌한 지점까지의 거리
    float Fraction;             // 적중이 발생한 광선의 거리의 비율
    Vec2 Normal;                // 충돌 표면의 노말벡터
    Vec2 Point;                 // 충돌 지점의 위치
    CGameObject* pCollisionObj; // 충돌한 콜라이더 오브젝트
};

// 충돌 콜백 클래스
class CCollision2DCallback : public b2ContactListener
{
public:
    virtual void BeginContact(b2Contact* contact) override;
    virtual void EndContact(b2Contact* contact) override;
};

class b2World;
class CGameObject;
class CCollider2D;

class CPhysics2DMgr : public CSingleton<CPhysics2DMgr>
{
    SINGLE(CPhysics2DMgr);

private:
    b2World* m_PhysicsWorld;
    CCollision2DCallback m_CallbackInst;
    vector<CGameObject*> m_vecPhysicsObj;

    WORD m_Matrix[LAYER_MAX];

    const float m_PPM; // PixelPerMeter
    float m_Accumulator;
    const float m_StepSize;

public:
    void tick();

    void OnPhysics2DStart();
    void OnPhysics2DStop();

public:
    void LayerCheck(UINT _LeftLayer, UINT _RightLayer, bool _bCheck = true);
    void LayerCheck(CLevel* _CurLevel, const wstring& _LeftLayer, const wstring& _RightLayer);

    void EnableAllLayer();
    void DisableAllLayer();

    WORD GetCollisionLayer(UINT idx) const { return m_Matrix[idx]; }
    void SetCollisionLayer(UINT idx, UINT row) { m_Matrix[idx] = row; }

    CGameObject* CollisionCheck(Vec2 _Point);
    RaycastHit2D RayCast(Vec2 _Origin, Vec2 _Dirction, float _Distance, WORD _LayerMask = 0xFFFF);
    RaycastHit2D RayCast(Vec2 _Origin, Vec2 _Dirction, float _Distance, const wstring& _LayerName);

public:
    const float GetPPM() const { return m_PPM; }

private:
    void AddPhysicsObject(CGameObject* _GameObject);
    void RemovePhysicsObject(CGameObject* _GameObject);

    friend class CTaskMgr;
    friend class CRigidbody2D;
};
