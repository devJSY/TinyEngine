#pragma once
#include <Engine\\CScript.h>

enum class ArrowType
{
    UP,
    DOWN,
};

class CElfilisAirArrow : public CScript
{
private:
    Ptr<CPrefab> m_CollisionEffect;
    CGameObject* m_Target;
    ArrowType m_Type;
    UINT m_Step;
    int m_ArrowIdx;
    float m_AccTime;

    // Init
    Vec3 m_InitPos;
    Vec3 m_InitDir[3];

    // Ready
    Vec3 m_ReadyDir;

    // Wait
    float m_WaitTime;

    // Attack
    float m_AttackSpeed;
    float m_TargetDist;
    bool m_bGround;

public:
    virtual void begin() override;
    virtual void tick() override;

    void StartSpawn();
    void StartReady();

public:
    bool IsSpawnFinished() { return m_Step != 1; }
    bool IsPlaying() { return m_Step != 0; }

    void SetTarget(CGameObject* _Target) { m_Target = _Target; }
    void SetInitPos(Vec3 _Pos) { m_InitPos = _Pos; }
    void SetInitDir(Vec3 _Front, Vec3 _Up, Vec3 _Right);
    void SetType(ArrowType _Type) { m_Type = _Type; }
    void SetArrowIdx(int _Idx);

private:
    void Spawn();
    void Ready();
    void Wait();
    void Aim();
    void Attack();

    void StartWait();
    void StartAim();
    void StartAttack();

    virtual void OnCollisionEnter(CCollider* _OtherCollider) override;
    virtual void OnCollisionStay(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisAirArrow);
    CElfilisAirArrow();
    virtual ~CElfilisAirArrow();
};
