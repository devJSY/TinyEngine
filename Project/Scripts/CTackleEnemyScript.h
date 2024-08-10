#pragma once
#include "CMonsterUnitScript.h"

enum class TACKLEENEMY_STATE
{
    Idle,
    Find,
    AttackPrev,
    Attack,
    AttackAfter,
    AttackAfter2,
    Wait,
    Damage,
    Eaten,
    Death,
};

class CTackleEnemyScript : public CMonsterUnitScript
{
private:
    TACKLEENEMY_STATE m_eState;

    Vec3 m_vDamageDir;
    
    float m_fAccTime;
    float m_fWaitTime;

    float m_fRushSpeedLerp;
    float m_fRushLerp;
    float m_fSpeed;
    float m_fMaxSpeed;

    float m_fThreshHoldRushLerp;

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
    void AttackAfter2();
    void Damage();
    void Eaten();
    void Wait();
    void Death();

private:
    Vec3 TrackDir(Vec3 _vPos);
    void ApplyDir(Vec3 _vFront, bool _flag);

public:
    CLONE(CTackleEnemyScript)
    CTackleEnemyScript();
    CTackleEnemyScript(const CTackleEnemyScript& Origin);
    virtual ~CTackleEnemyScript();
};
