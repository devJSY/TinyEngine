#pragma once
#include "CMonsterUnitScript.h"

enum class PhantaState
{
    Appear,
    Wait,
    Find,
    Attack,
    Brake,
    Damage,
    Disappear,
    End,
};

class CPhantaScript : public CMonsterUnitScript
{
private:
    PhantaState m_eState;
    float m_fRushSpeedLerp;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void EnterState();
    void FSM();
    void ExitState();
    void ChangeState(PhantaState _state);
    void CheckDamage();

private:
    void Appear();
    void Wait();
    void Find();
    void Attack();
    void Brake();
    void Damage();
    void Disappear();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    CLONE(CPhantaScript)

public:
    CPhantaScript();
    CPhantaScript(const CPhantaScript& Origin);
    virtual ~CPhantaScript();
};
