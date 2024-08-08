#pragma once
#include "CMonsterUnitScript.h"

enum class TACKLEENEMY_STATE
{
    Idle,
    Patrol,
    Find,
    AttackPrev,
    Attack,
    AttackAfter,
    Damage,
    Eaten,
    Death,
};

class CTackleEnemyScript : public CMonsterUnitScript
{
private:
    TACKLEENEMY_STATE m_eState;

    CGameObject* m_pTargetObject;
    float m_fRushSpeedLerp;
    float m_fRushLerp;
    float m_fSpeed;
    float m_fMaxSpeed;

    float m_fThreshHoldRushLerp;
    float m_fPatrolTime;
    float m_fPatrolAccTime;

    Vec3 m_vDamageDir;
    bool m_bFlag;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void ChangeState(TACKLEENEMY_STATE _state);
    void EnterState(TACKLEENEMY_STATE _state);
    void ExitState(TACKLEENEMY_STATE _state);

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

private:
    void Idle();
    void Find();
    void AttackPrev();
    void Attack();
    void AttackAfter();
    void Damage();
    void Eaten();
    void Death();

private:
    Vec3 TrackDir(Vec3 _vPos);
    TACKLEENEMY_STATE RandomIdleState();
    void ApplyDir(Vec3 _vFront, bool _flag);

public:
    CLONE(CTackleEnemyScript)
    CTackleEnemyScript();
    CTackleEnemyScript(const CTackleEnemyScript& Origin);
    virtual ~CTackleEnemyScript();
};
