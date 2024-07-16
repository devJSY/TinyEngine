#pragma once
#include "CMonsterUnitScript.h"

enum class BLADEKNIGHT_STATE
{
    Attack,
    AttackStart,
    Damage,
    DoubleAttack,
    Fall,
    Find,
    FindWait,
    FindWaitSub,
    Landing,
    Move,
    Retreat,
    Thrust,
    ThrustEnd,
    ThrustLoop,
    ThrustStart,
    ThrustStartWait,
    ThrustWait,
    TonadoAttack,
    TonadoAttackCharge,
    TonadoAttackCharge2,
    TonadoAttackChargeMax,
    Wait,
};

class CBladeKnightScript : public CMonsterUnitScript
{
private:
    BLADEKNIGHT_STATE m_State;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    void ChangeState(BLADEKNIGHT_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Attack();
    void AttackStart();
    void Damage();
    void DoubleAttack();
    void Fall();
    void Find();
    void FindWait();
    void FindWaitSub();
    void Landing();
    void Move();
    void Retreat();
    void Thrust();
    void ThrustEnd();
    void ThrustLoop();
    void ThrustStart();
    void ThrustStartWait();
    void ThrustWait();
    void TonadoAttack();
    void TonadoAttackCharge();
    void TonadoAttackCharge2();
    void TonadoAttackChargeMax();
    void Wait();

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider);
    virtual void OnCollisionStay(CCollider* _OtherCollider);
    virtual void OnCollisionExit(CCollider* _OtherCollider);

    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBladeKnightScript);

public:
    CBladeKnightScript();
    CBladeKnightScript(const CBladeKnightScript& origin);
    virtual ~CBladeKnightScript();
};
