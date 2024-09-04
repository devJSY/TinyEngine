#pragma once
#include "CMonsterUnitScript.h"

enum class TackleEnemyState
{
    Idle,
    Find,
    AttackPrev,
    Attack,
    AttackAfter,
    AttackAfter2,
    Wait,
    Fall,
    Landing,
    Damage,
    Eaten,
    Death,
};

class CTackleEnemyScript : public CMonsterUnitScript
{
private:
    TackleEnemyState m_eState;
    CGameObject* m_pDashEffect;

    float m_fAccTime;
    float m_fWaitTime;

    float m_fRushSpeedLerp;
    float m_fRushLerp;
    float m_fSpeed;
    float m_fMaxSpeed;

    float m_fThreshHoldRushLerp;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void EnterState(TackleEnemyState _state);
    void FSM();
    void ExitState(TackleEnemyState _state);
    void ChangeState(TackleEnemyState _state);
    void CheckDamage();

    Vec3 TrackDir(Vec3 _vPos);
    void ApplyDir(Vec3 _vFront, bool _flag);
    void DashEffectOn();
    void DashEffectOff();

private:
    void OnTriggerEnter(CCollider* _OtherCollider);

private:
    void Idle();
    void Find();
    void AttackPrev();
    void Attack();
    void AttackAfter();
    void AttackAfter2();
    void Fall();
    void Landing();
    void Damage();
    void Eaten();
    void Wait();
    void Death();

public:
    CLONE(CTackleEnemyScript)
    CTackleEnemyScript();
    CTackleEnemyScript(const CTackleEnemyScript& Origin);
    virtual ~CTackleEnemyScript();
};
