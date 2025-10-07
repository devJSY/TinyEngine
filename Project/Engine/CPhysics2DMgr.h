#pragma once

#include <box2d/b2_world_callbacks.h>

// RayCast
struct RaycastHit2D
{
    Vec2 Centroid;              // Raycast �߽� ��ġ
    float Distance;             // Centroid ���� ���� Ray�� �߻��Ͽ� �浹�� ���������� �Ÿ�
    float Fraction;             // ������ �߻��� ������ �Ÿ��� ����
    Vec2 Normal;                // �浹 ǥ���� �븻����
    Vec2 Point;                 // �浹 ������ ��ġ
    CGameObject* pCollisionObj; // �浹�� �ݶ��̴� ������Ʈ
};

// �浹 �ݹ� Ŭ����
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
    // Box2D
    b2World* m_PhysicsWorld;
    CCollision2DCallback m_CallbackInst;

private:
    vector<CGameObject*> m_vecPhysicsObj;

    WORD m_Matrix[LAYER_MAX];

    float m_Accumulator;
    const float m_StepSize;

    Vec2 m_Gravity;
    float m_PPM; // PixelPerMeter

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
    RaycastHit2D RayCast(Vec2 _Origin, Vec2 _Direction, float _Distance, WORD _LayerMask = 0xFFFF);
    RaycastHit2D RayCast(Vec2 _Origin, Vec2 _Direction, float _Distance, const vector<wstring>& _LayerNames);

public:
    Vec2 GetGravity() const { return m_Gravity; }
    void SetGravity(Vec2 _Gravity) { m_Gravity = _Gravity; }

    float GetPPM() const { return m_PPM; }
    void SetPPM(float _PPM) { m_PPM = _PPM; }

private:
    void AddPhysicsObject(CGameObject* _GameObject);
    void RemovePhysicsObject(CGameObject* _GameObject);

    friend class CTaskMgr;
    friend class CRigidbody2D;
};
