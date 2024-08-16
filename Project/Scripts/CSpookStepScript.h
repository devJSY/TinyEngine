#pragma once

#include "CMonsterUnitScript.h"
enum class SpookStepState
{
    Appear,
    Wait,
    Find,
    Move,
    Fall,
    Landing,
    Damage,
    Disappear,
    Eaten,
    End,
};

class CSpookStepScript : public CMonsterUnitScript
{
private:
    SpookStepState m_eState;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void EnterState();
    void FSM();
    void ExitState();
    void ChangeState(SpookStepState _state);
    void CheckDamage();

private:
    void Appear();
    void Wait();
    void Find();
    void Move();
    void Fall();
    void Landing();
    void Damage();
    void Eaten();
    void Disappear();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    CLONE(CSpookStepScript)

public:
    CSpookStepScript();
    CSpookStepScript(const CSpookStepScript& Origin);
    virtual ~CSpookStepScript();
};
