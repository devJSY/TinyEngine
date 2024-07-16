#include "pch.h"
#include "CBladeKnightScript.h"

CBladeKnightScript::CBladeKnightScript()
    : CMonsterUnitScript(BLADEKNIGHTSCRIPT)
    , m_State(BLADEKNIGHT_STATE::Wait)
{
}

CBladeKnightScript::CBladeKnightScript(const CBladeKnightScript& origin)
    : CMonsterUnitScript(origin)
    , m_State(origin.m_State)
{
}

CBladeKnightScript::~CBladeKnightScript()
{
}

void CBladeKnightScript::begin()
{
    CMonsterUnitScript::begin();

    ChangeState(m_State);
}

void CBladeKnightScript::tick()
{
    CMonsterUnitScript::tick();

    // FSM
    switch (m_State)
    {
    case BLADEKNIGHT_STATE::Attack:
        Attack();
        break;
    case BLADEKNIGHT_STATE::AttackStart:
        AttackStart();
        break;
    case BLADEKNIGHT_STATE::Damage:
        Damage();
        break;
    case BLADEKNIGHT_STATE::DoubleAttack:
        DoubleAttack();
        break;
    case BLADEKNIGHT_STATE::Fall:
        Fall();
        break;
    case BLADEKNIGHT_STATE::Find:
        Find();
        break;
    case BLADEKNIGHT_STATE::FindWait:
        FindWait();
        break;
    case BLADEKNIGHT_STATE::FindWaitSub:
        FindWaitSub();
        break;
    case BLADEKNIGHT_STATE::Landing:
        Landing();
        break;
    case BLADEKNIGHT_STATE::Move:
        Move();
        break;
    case BLADEKNIGHT_STATE::Retreat:
        Retreat();
        break;
    case BLADEKNIGHT_STATE::Thrust:
        Thrust();
        break;
    case BLADEKNIGHT_STATE::ThrustEnd:
        ThrustEnd();
        break;
    case BLADEKNIGHT_STATE::ThrustLoop:
        ThrustLoop();
        break;
    case BLADEKNIGHT_STATE::ThrustStart:
        ThrustStart();
        break;
    case BLADEKNIGHT_STATE::ThrustStartWait:
        ThrustStartWait();
        break;
    case BLADEKNIGHT_STATE::ThrustWait:
        ThrustWait();
        break;
    case BLADEKNIGHT_STATE::TonadoAttack:
        TonadoAttack();
        break;
    case BLADEKNIGHT_STATE::TonadoAttackCharge:
        TonadoAttackCharge();
        break;
    case BLADEKNIGHT_STATE::TonadoAttackCharge2:
        TonadoAttackCharge2();
        break;
    case BLADEKNIGHT_STATE::TonadoAttackChargeMax:
        TonadoAttackChargeMax();
        break;
    case BLADEKNIGHT_STATE::Wait:
        Wait();
        break;
    }
}

void CBladeKnightScript::ChangeState(BLADEKNIGHT_STATE _NextState)
{
    ExitState();
    m_State = _NextState;
    EnterState();
}

void CBladeKnightScript::EnterState()
{
    switch (m_State)
    {
    case BLADEKNIGHT_STATE::Attack: {
        Animator()->Play(ANIMPREFIX(L"Attack"), false);
    }
    break;
    case BLADEKNIGHT_STATE::AttackStart: {
        Animator()->Play(ANIMPREFIX(L"AttackStart"), false);
    }
    break;
    case BLADEKNIGHT_STATE::Damage: {
        Animator()->Play(ANIMPREFIX(L"Damage"), false);
    }
    break;
    case BLADEKNIGHT_STATE::DoubleAttack: {
        Animator()->Play(ANIMPREFIX(L"DoubleAttack"), false);
    }
    break;
    case BLADEKNIGHT_STATE::Fall: {
        Animator()->Play(ANIMPREFIX(L"Fall"), true);
    }
    break;
    case BLADEKNIGHT_STATE::Find: {
        Animator()->Play(ANIMPREFIX(L"Find"), false);
    }
    break;
    case BLADEKNIGHT_STATE::FindWait: {
        Animator()->Play(ANIMPREFIX(L"FindWait"));
    }
    break;
    case BLADEKNIGHT_STATE::FindWaitSub: {
        Animator()->Play(ANIMPREFIX(L"FindWaitSub"));
    }
    break;
    case BLADEKNIGHT_STATE::Landing: {
        Animator()->Play(ANIMPREFIX(L"Landing"), false);
    }
    break;
    case BLADEKNIGHT_STATE::Move: {
        Animator()->Play(ANIMPREFIX(L"Move"));
    }
    break;
    case BLADEKNIGHT_STATE::Retreat: {
        Animator()->Play(ANIMPREFIX(L"Retreat"), false);
    }
    break;
    case BLADEKNIGHT_STATE::Thrust: {
        Animator()->Play(ANIMPREFIX(L"Thrust"), false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustEnd: {
        Animator()->Play(ANIMPREFIX(L"ThrustEnd"), false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustLoop: {
        Animator()->Play(ANIMPREFIX(L"ThrustLoop"));
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStart: {
        Animator()->Play(ANIMPREFIX(L"ThrustStart"), false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStartWait: {
        Animator()->Play(ANIMPREFIX(L"ThrustStartWait"));
    }
    break;
    case BLADEKNIGHT_STATE::ThrustWait: {
        Animator()->Play(ANIMPREFIX(L"ThrustWait"));
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttack: {
        Animator()->Play(ANIMPREFIX(L"TonadoAttack"), false);
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttackCharge: {
        Animator()->Play(ANIMPREFIX(L"TonadoAttackCharge"), false);
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttackCharge2: {
        Animator()->Play(ANIMPREFIX(L"TonadoAttackCharge2"), false);
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttackChargeMax: {
        Animator()->Play(ANIMPREFIX(L"TonadoAttackChargeMax"));
    }
    break;
    case BLADEKNIGHT_STATE::Wait: {
        Animator()->Play(ANIMPREFIX(L"Wait"));
    }
    break;
    }
}

void CBladeKnightScript::ExitState()
{
    switch (m_State)
    {
    case BLADEKNIGHT_STATE::Attack: {
    }
    break;
    case BLADEKNIGHT_STATE::AttackStart: {
    }
    break;
    case BLADEKNIGHT_STATE::Damage: {
    }
    break;
    case BLADEKNIGHT_STATE::DoubleAttack: {
    }
    break;
    case BLADEKNIGHT_STATE::Fall: {
    }
    break;
    case BLADEKNIGHT_STATE::Find: {
    }
    break;
    case BLADEKNIGHT_STATE::FindWait: {
    }
    break;
    case BLADEKNIGHT_STATE::FindWaitSub: {
    }
    break;
    case BLADEKNIGHT_STATE::Landing: {
    }
    break;
    case BLADEKNIGHT_STATE::Move: {
    }
    break;
    case BLADEKNIGHT_STATE::Retreat: {
    }
    break;
    case BLADEKNIGHT_STATE::Thrust: {
    }
    break;
    case BLADEKNIGHT_STATE::ThrustEnd: {
    }
    break;
    case BLADEKNIGHT_STATE::ThrustLoop: {
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStart: {
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStartWait: {
    }
    break;
    case BLADEKNIGHT_STATE::ThrustWait: {
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttack: {
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttackCharge: {
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttackCharge2: {
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttackChargeMax: {
    }
    break;
    case BLADEKNIGHT_STATE::Wait: {
    }
    break;
    }
}

void CBladeKnightScript::Attack()
{
}

void CBladeKnightScript::AttackStart()
{
}

void CBladeKnightScript::Damage()
{
}

void CBladeKnightScript::DoubleAttack()
{
}

void CBladeKnightScript::Fall()
{
}

void CBladeKnightScript::Find()
{
}

void CBladeKnightScript::FindWait()
{
}

void CBladeKnightScript::FindWaitSub()
{
}

void CBladeKnightScript::Landing()
{
}

void CBladeKnightScript::Move()
{
}

void CBladeKnightScript::Retreat()
{
}

void CBladeKnightScript::Thrust()
{
}

void CBladeKnightScript::ThrustEnd()
{
}

void CBladeKnightScript::ThrustLoop()
{
}

void CBladeKnightScript::ThrustStart()
{
}

void CBladeKnightScript::ThrustStartWait()
{
}

void CBladeKnightScript::ThrustWait()
{
}

void CBladeKnightScript::TonadoAttack()
{
}

void CBladeKnightScript::TonadoAttackCharge()
{
}

void CBladeKnightScript::TonadoAttackCharge2()
{
}

void CBladeKnightScript::TonadoAttackChargeMax()
{
}

void CBladeKnightScript::Wait()
{
}

void CBladeKnightScript::OnCollisionEnter(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnCollisionStay(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnCollisionExit(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnTriggerEnter(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnTriggerStay(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CBladeKnightScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}
