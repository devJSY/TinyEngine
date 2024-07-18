#include "pch.h"
#include "CNoddyScript.h"

CNoddyScript::CNoddyScript()
    : CMonsterUnitScript(NODDYSCRIPT)
{
}

CNoddyScript::CNoddyScript(const CNoddyScript& origin)
    : CMonsterUnitScript(origin)
{
}

CNoddyScript::~CNoddyScript()
{
}

void CNoddyScript::begin()
{
}

void CNoddyScript::tick()
{
    // FSM
    switch (m_State)
    {
    case NODDY_STATE::Damage:
        Damage();
        break;
    case NODDY_STATE::Fall:
        Fall();
        break;
    case NODDY_STATE::FallSleep:
        FallSleep();
        break;
    case NODDY_STATE::Landing:
        Landing();
        break;
    case NODDY_STATE::LandingSleep:
        LandingSleep();
        break;
    case NODDY_STATE::LookAround:
        LookAround();
        break;
    case NODDY_STATE::Move:
        Move();
        break;
    case NODDY_STATE::Sleep:
        Sleep();
        break;
    case NODDY_STATE::SleepStart:
        SleepStart();
        break;
    case NODDY_STATE::Wait:
        Wait();
        break;
    case NODDY_STATE::Wakeup:
        Wakeup();
        break;
    }
}

void CNoddyScript::ChangeState(NODDY_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CNoddyScript::EnterState()
{
    // FSM
    switch (m_State)
    {
    case NODDY_STATE::Damage: {
        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case NODDY_STATE::Fall: {
        Animator()->Play(ANIMPREFIX("Fall"));
    }
    break;
    case NODDY_STATE::FallSleep: {
        Animator()->Play(ANIMPREFIX("FallSleep"));
    }
    break;
    case NODDY_STATE::Landing: {
        Animator()->Play(ANIMPREFIX("Landing"), false);
    }
    break;
    case NODDY_STATE::LandingSleep: {
        Animator()->Play(ANIMPREFIX("LandingSleep"));
    }
    break;
    case NODDY_STATE::LookAround: {
        Animator()->Play(ANIMPREFIX("LookAround"), false);
    }
    break;
    case NODDY_STATE::Move: {
        Animator()->Play(ANIMPREFIX("Move"));
    }
    break;
    case NODDY_STATE::Sleep: {
        Animator()->Play(ANIMPREFIX("Sleep"));
    }
    break;
    case NODDY_STATE::SleepStart: {
        Animator()->Play(ANIMPREFIX("SleepStart"), false);
    }
    break;
    case NODDY_STATE::Wait: {
        Animator()->Play(ANIMPREFIX("Wait"));
    }
    break;
    case NODDY_STATE::Wakeup: {
        Animator()->Play(ANIMPREFIX("Wakeup"), false);
    }
    break;
    }
}

void CNoddyScript::ExitState()
{
    // FSM
    switch (m_State)
    {
    case NODDY_STATE::Damage: {
    }
    break;
    case NODDY_STATE::Fall: {
    }
    break;
    case NODDY_STATE::FallSleep: {
    }
    break;
    case NODDY_STATE::Landing: {
    }
    break;
    case NODDY_STATE::LandingSleep: {
    }
    break;
    case NODDY_STATE::LookAround: {
    }
    break;
    case NODDY_STATE::Move: {
    }
    break;
    case NODDY_STATE::Sleep: {
    }
    break;
    case NODDY_STATE::SleepStart: {
    }
    break;
    case NODDY_STATE::Wait: {
    }
    break;
    case NODDY_STATE::Wakeup: {
    }
    break;
    }
}

void CNoddyScript::Damage()
{
}

void CNoddyScript::Fall()
{
}

void CNoddyScript::FallSleep()
{
}

void CNoddyScript::Landing()
{
}

void CNoddyScript::LandingSleep()
{
}

void CNoddyScript::LookAround()
{
}

void CNoddyScript::Move()
{
}

void CNoddyScript::Sleep()
{
}

void CNoddyScript::SleepStart()
{
}

void CNoddyScript::Wait()
{
}

void CNoddyScript::Wakeup()
{
}

void CNoddyScript::OnCollisionEnter(CCollider* _OtherCollider)
{
}

void CNoddyScript::OnCollisionStay(CCollider* _OtherCollider)
{
}

void CNoddyScript::OnCollisionExit(CCollider* _OtherCollider)
{
}
