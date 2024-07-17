#include "pch.h"
#include "CBladeKnightScript.h"

CBladeKnightScript::CBladeKnightScript()
    : CMonsterUnitScript(BLADEKNIGHTSCRIPT)
    , m_State(BLADEKNIGHT_STATE::Wait)
    , m_PassedTime(0.f)
{
}

CBladeKnightScript::CBladeKnightScript(const CBladeKnightScript& origin)
    : CMonsterUnitScript(origin)
    , m_State(origin.m_State)
    , m_PassedTime(origin.m_PassedTime)
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
    m_PassedTime = 0.f;
    EnterState();
}

void CBladeKnightScript::EnterState()
{
    switch (m_State)
    {
    case BLADEKNIGHT_STATE::Attack: {
        Animator()->Play(ANIMPREFIX("Attack"), false);
    }
    break;
    case BLADEKNIGHT_STATE::AttackStart: {
        Animator()->Play(ANIMPREFIX("AttackStart"), false);
    }
    break;
    case BLADEKNIGHT_STATE::Damage: {
        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case BLADEKNIGHT_STATE::DoubleAttack: {
        Animator()->Play(ANIMPREFIX("DoubleAttack"), false);
    }
    break;
    case BLADEKNIGHT_STATE::Fall: {
        Animator()->Play(ANIMPREFIX("Fall"), true);
    }
    break;
    case BLADEKNIGHT_STATE::Find: {
        Animator()->Play(ANIMPREFIX("Find"), false, false, 1.f);
    }
    break;
    case BLADEKNIGHT_STATE::FindWait: {
        Animator()->Play(ANIMPREFIX("FindWait"));
    }
    break;
    case BLADEKNIGHT_STATE::FindWaitSub: {
        Animator()->Play(ANIMPREFIX("FindWaitSub"), true, false, 1.f);
    }
    break;
    case BLADEKNIGHT_STATE::Landing: {
        Animator()->Play(ANIMPREFIX("Landing"), false, false, 1.f);
    }
    break;
    case BLADEKNIGHT_STATE::Move: {
        Animator()->Play(ANIMPREFIX("Move"));
    }
    break;
    case BLADEKNIGHT_STATE::Retreat: {
        Animator()->Play(ANIMPREFIX("Retreat"), false);
    }
    break;
    case BLADEKNIGHT_STATE::Thrust: {
        Animator()->Play(ANIMPREFIX("Thrust"), false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustEnd: {
        Animator()->Play(ANIMPREFIX("ThrustEnd"), false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustLoop: {
        Animator()->Play(ANIMPREFIX("ThrustLoop"));
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStart: {
        Animator()->Play(ANIMPREFIX("ThrustStart"), false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStartWait: {
        Animator()->Play(ANIMPREFIX("ThrustStartWait"));
    }
    break;
    case BLADEKNIGHT_STATE::ThrustWait: {
        Animator()->Play(ANIMPREFIX("ThrustWait"), true, false, 2.5f, 0.5);
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttack: {
        Animator()->Play(ANIMPREFIX("TonadoAttack"), false);
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttackCharge: {
        Animator()->Play(ANIMPREFIX("TonadoAttackCharge"), false);
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttackCharge2: {
        Animator()->Play(ANIMPREFIX("TonadoAttackCharge2"), false);
    }
    break;
    case BLADEKNIGHT_STATE::TonadoAttackChargeMax: {
        Animator()->Play(ANIMPREFIX("TonadoAttackChargeMax"));
    }
    break;
    case BLADEKNIGHT_STATE::Wait: {
        Animator()->Play(ANIMPREFIX("Wait"), true, false, 2.5f, 0.5);
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
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Rigidbody()->SetAngularVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case BLADEKNIGHT_STATE::Landing: {
    }
    break;
    case BLADEKNIGHT_STATE::Move: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Rigidbody()->SetAngularVelocity(Vec3(0.f, 0.f, 0.f));
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
    if (IsGround())
    {
        ChangeState(BLADEKNIGHT_STATE::Landing);
    }
}

void CBladeKnightScript::Find()
{
    TransformRotate();

    if (Animator()->IsFinish())
    {
        ChangeState(BLADEKNIGHT_STATE::FindWait);
    }
}

void CBladeKnightScript::FindWait()
{
    m_PassedTime += DT;

    if (m_PassedTime > 3.f)
    {
        ChangeState(BLADEKNIGHT_STATE::Wait);
    }

    if (nullptr != GetTarget() && m_PassedTime > 1.f)
    {
        ChangeState(BLADEKNIGHT_STATE::FindWaitSub);
    }

    if (!IsGround())
    {
        ChangeState(BLADEKNIGHT_STATE::Fall);
    }
}

void CBladeKnightScript::FindWaitSub()
{
    RigidbodyMove(GetTarget());
    TransformRotate();

    if (nullptr == GetTarget())
    {
        ChangeState(BLADEKNIGHT_STATE::FindWait);
    }

    if (!IsGround())
    {
        ChangeState(BLADEKNIGHT_STATE::Fall);
    }
}

void CBladeKnightScript::Landing()
{
    if (Animator()->IsFinish())
    {
        if (nullptr == GetTarget())
        {
            ChangeState(BLADEKNIGHT_STATE::Wait);
        }
        else
        {
            ChangeState(BLADEKNIGHT_STATE::FindWait);
        }
    }
}

void CBladeKnightScript::Move()
{
    RigidbodyMove(GetTarget());

    m_PassedTime += DT;

    if (m_PassedTime > 1.f)
    {
        ChangeState(BLADEKNIGHT_STATE::Wait);
    }

    if (nullptr != GetTarget())
    {
        ChangeState(BLADEKNIGHT_STATE::Find);
    }

    if (!IsGround())
    {
        ChangeState(BLADEKNIGHT_STATE::Fall);
    }
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
    m_PassedTime += DT;

    if (m_PassedTime > 5.f)
    {
        ChangeState(BLADEKNIGHT_STATE::Move);
    }

    if (nullptr != GetTarget())
    {
        ChangeState(BLADEKNIGHT_STATE::Find);
    }

    if (!IsGround())
    {
        ChangeState(BLADEKNIGHT_STATE::Fall);
    }
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

void CBladeKnightScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CBladeKnightScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}
