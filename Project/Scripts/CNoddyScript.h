#pragma once
#include "CMonsterUnitScript.h"

enum class NODDY_STATE
{
    Damage,
    Fall,
    FallSleep,
    Landing,
    LandingSleep,
    LookAround,
    Move,
    Sleep,
    SleepStart,
    Wait,
    Wakeup
};

class CNoddyScript : public CMonsterUnitScript
{
private:
    NODDY_STATE m_State;
    float m_PassedTime;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    void ChangeState(NODDY_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Damage();
    void Fall();
    void FallSleep();
    void Landing();
    void LandingSleep();
    void LookAround();
    void Move();
    void Sleep();
    void SleepStart();
    void Wait();
    void Wakeup();

private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider);
    virtual void OnCollisionStay(CCollider* _OtherCollider);
    virtual void OnCollisionExit(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CNoddyScript);

public:
    CNoddyScript();
    CNoddyScript(const CNoddyScript& origin);
    virtual ~CNoddyScript();
};
