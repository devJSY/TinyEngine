#pragma once
#include <Engine/CScript.h>

#include "CMonsterUnitScript.h"

enum class NormalEnemyState
{
    Idle,
    Grooming,
    Patrol,
    Sleep,
    Find,
    Attack,
    Brake,
    AfterAttack,
    Damage,
    Fall,
    Land,
    Eaten,
    Death,
    End,
};

class CNormalEnemyScript : public CMonsterUnitScript
{
private:
    Vec3 m_vPatrolDir;
    Vec3 m_vDamageDir;
    Vec3 m_vCenterPoint;

    NormalEnemyState m_ePrevState;
    NormalEnemyState m_eState;

    float m_fMaxSpeed;
    float m_fSpeed;
    float m_fRushLerp;
    float m_fRushSpeedLerp;
    float m_fThreshHoldRushSpeedLerp;

    bool m_bEnter;
    bool m_bFirst;
    bool m_bCirclePatrol;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void EnterState(NormalEnemyState _state);
    void FSM();
    void ExitState(NormalEnemyState _state);
    void ChangeState(NormalEnemyState _state);
    void CheckDamage();
    NormalEnemyState RandomIdleState();
    Vec3 TrackDir(Vec3 _vPos);
    void ApplyDir(Vec3 _vFront, bool _flag);
    void PatrolMove();

private:
    void Idle();
    void Patrol();
    void Find();
    void Attack();
    void Grooming();
    void Brake();
    void Fall();
    void Land();
    void AfterAttack();
    void Eaten();
    void Damage();
    void Death();

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    CLONE(CNormalEnemyScript)
    CNormalEnemyScript();
    CNormalEnemyScript(const CNormalEnemyScript& Origin);
    virtual ~CNormalEnemyScript();
};
