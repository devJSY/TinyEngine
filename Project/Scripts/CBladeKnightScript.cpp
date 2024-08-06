#include "pch.h"
#include "CBladeKnightScript.h"
#include "CBladeKnightSwordScript.h"

CBladeKnightScript::CBladeKnightScript()
    : CMonsterUnitScript(BLADEKNIGHTSCRIPT)
    , m_State(BLADEKNIGHT_STATE::Wait)
    , m_Sword(nullptr)
    , m_PassedTime(0.f)
    , m_bStepFlag(false)
    , m_StepPower(10.f)
    , m_AttackRange(50.f)
    , m_bTraceUnit(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_StepPower, "Step Power");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_AttackRange, "Attack Range");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bTraceUnit, "Trace Unit");
}

CBladeKnightScript::CBladeKnightScript(const CBladeKnightScript& origin)
    : CMonsterUnitScript(origin)
    , m_State(origin.m_State)
    , m_Sword(nullptr)
    , m_PassedTime(origin.m_PassedTime)
    , m_bStepFlag(false)
    , m_StepPower(origin.m_StepPower)
    , m_AttackRange(origin.m_AttackRange)
    , m_bTraceUnit(origin.m_bTraceUnit)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_StepPower, "Step Power");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_AttackRange, "Attack Range");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bTraceUnit, "Trace Unit");
}

CBladeKnightScript::~CBladeKnightScript()
{
}

void CBladeKnightScript::begin()
{
    CMonsterUnitScript::begin();

    SetSword();

    ChangeState(m_State);
}

void CBladeKnightScript::tick()
{
    // Damage Proc
    ClearHitDir();
    m_PrevInfo = m_CurInfo;

    float DamageAmount = DamageProc();
    if (DamageAmount > 0.f)
    {
        m_CurInfo.HP -= DamageAmount;

        // 공격 상태가 아닌 경우에만 Damage상태로 변경
        switch (m_State)
        {
        case BLADEKNIGHT_STATE::Fall:
        case BLADEKNIGHT_STATE::Find:
        case BLADEKNIGHT_STATE::FindWait:
        case BLADEKNIGHT_STATE::FindWaitSub:
        case BLADEKNIGHT_STATE::Landing:
        case BLADEKNIGHT_STATE::Move:
        case BLADEKNIGHT_STATE::Retreat:
        case BLADEKNIGHT_STATE::Wait: {
            ChangeState(BLADEKNIGHT_STATE::Damage);
        }
        break;
        default:
            break;
        }
    }

    // Sword
    if (nullptr == m_Sword)
    {
        SetSword();
    }

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
    // case BLADEKNIGHT_STATE::ThrustWait:
    //     ThrustWait();
    //     break;
    case BLADEKNIGHT_STATE::TornadoAttack:
        TornadoAttack();
        break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge:
        TornadoAttackCharge();
        break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge2:
        TornadoAttackCharge2();
        break;
    // case BLADEKNIGHT_STATE::TornadoAttackChargeMax:
    //     TornadoAttackChargeMax();
    //     break;
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

void CBladeKnightScript::SetSword()
{
    for (CGameObject* pChild : GetOwner()->GetChildObject())
    {
        CBladeKnightSwordScript* Sword = pChild->GetScript<CBladeKnightSwordScript>();
        if (nullptr != Sword)
        {
            m_Sword = Sword;
            break;
        }
    }
}

void CBladeKnightScript::EnterState()
{
    switch (m_State)
    {
    case BLADEKNIGHT_STATE::Attack: {
        Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_StepPower, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("Attack"), false, false, 1.5f);
    }
    break;
    case BLADEKNIGHT_STATE::AttackStart: {
        Animator()->Play(ANIMPREFIX("AttackStart"), false);
    }
    break;
    case BLADEKNIGHT_STATE::Damage: {
        // 피격 방향으로 회전
        Transform()->Slerp(GetHitDir(), 1.f); 

        // 피격 방향으로 Impulse
        Vec3 Impulse = GetHitDir();
        Impulse.Normalize();
        Impulse *= 5.f;
        Rigidbody()->AddForce(Impulse, ForceMode::Impulse);
        
        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case BLADEKNIGHT_STATE::DoubleAttack: {
        Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_StepPower, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("DoubleAttack"), false, false, 1.5f);
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
        Animator()->Play(ANIMPREFIX("FindWaitSub"));
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
        Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_StepPower * 2.f, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("Thrust"), false);
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::Thrust);
        }
    }
    break;
    case BLADEKNIGHT_STATE::ThrustEnd: {
        Animator()->Play(ANIMPREFIX("ThrustEnd"), false, false, 1.f);
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::ThrustEnd);
        }
    }
    break;
    case BLADEKNIGHT_STATE::ThrustLoop: {
        Animator()->Play(ANIMPREFIX("ThrustLoop"));
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::ThrustLoop);
        }
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStart: {
        Animator()->Play(ANIMPREFIX("ThrustStart"), false);
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::ThrustStart);
        }
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStartWait: {
        Animator()->Play(ANIMPREFIX("ThrustStartWait"));
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::ThrustStartWait);
        }
    }
    break;
    // case BLADEKNIGHT_STATE::ThrustWait: {
    //     Animator()->Play(ANIMPREFIX("ThrustWait"), true, false, 2.5f, 0.5);
    //     if (nullptr != m_Sword)
    //     {
    //         m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::ThrustWait);
    //     }
    // }
    // break;
    case BLADEKNIGHT_STATE::TornadoAttack: {
        Animator()->Play(ANIMPREFIX("TornadoAttack"), false, false, 1.5f);
    }
    break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge: {
        Animator()->Play(ANIMPREFIX("TornadoAttackCharge"), false);
    }
    break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge2: {
        Animator()->Play(ANIMPREFIX("TornadoAttackCharge2"), false);
    }
    break;
    // case BLADEKNIGHT_STATE::TornadoAttackChargeMax: {
    //     Animator()->Play(ANIMPREFIX("TornadoAttackChargeMax"), false, false, 2.5f, 0.);
    // }
    // break;
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
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case BLADEKNIGHT_STATE::AttackStart: {
    }
    break;
    case BLADEKNIGHT_STATE::Damage: {
        if (m_CurInfo.HP <= 0.f)
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Rigidbody()->SetAngularVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case BLADEKNIGHT_STATE::DoubleAttack: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case BLADEKNIGHT_STATE::Fall: {
    }
    break;
    case BLADEKNIGHT_STATE::Find: {
        Rigidbody()->SetAngularVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case BLADEKNIGHT_STATE::FindWait: {
        Rigidbody()->SetAngularVelocity(Vec3(0.f, 0.f, 0.f));
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
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case BLADEKNIGHT_STATE::Thrust: {
    }
    break;
    case BLADEKNIGHT_STATE::ThrustEnd: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case BLADEKNIGHT_STATE::ThrustLoop: {
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStart: {
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStartWait: {
        Rigidbody()->SetAngularVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    // case BLADEKNIGHT_STATE::ThrustWait: {
    // }
    // break;
    case BLADEKNIGHT_STATE::TornadoAttack: {
    }
    break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge: {
    }
    break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge2: {
    }
    break;
    // case BLADEKNIGHT_STATE::TornadoAttackChargeMax: {
    // }
    // break;
    case BLADEKNIGHT_STATE::Wait: {
    }
    break;
    }
}

void CBladeKnightScript::Attack()
{
    if (CHECK_ANIMFRM(GetOwner(), 10))
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }

    if (Animator()->IsFinish())
    {
        ChangeState(BLADEKNIGHT_STATE::Retreat);
    }
}

void CBladeKnightScript::AttackStart()
{
    m_PassedTime += DT;

    if (m_PassedTime > 1.f)
    {
        int RandomInt = GetRandomInt(0, 1);
        if (0 == RandomInt)
        {
            ChangeState(BLADEKNIGHT_STATE::Attack);
        }
        else
        {
            ChangeState(BLADEKNIGHT_STATE::DoubleAttack);
        }
    }
}

void CBladeKnightScript::Damage()
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

void CBladeKnightScript::DoubleAttack()
{
    if (CHECK_ANIMFRM(GetOwner(), 10) || CHECK_ANIMFRM(GetOwner(), 40))
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    else if (CHECK_ANIMFRM(GetOwner(), 20) && !m_bStepFlag)
    {
        Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_StepPower, ForceMode::Impulse);
        m_bStepFlag = true;
    }

    if (Animator()->IsFinish())
    {
        m_bStepFlag = false;
        ChangeState(BLADEKNIGHT_STATE::Retreat);
    }
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
    RotatingToTarget();

    if (Animator()->IsFinish())
    {
        ChangeState(BLADEKNIGHT_STATE::FindWait);
    }
}

void CBladeKnightScript::FindWait()
{
    RotatingToTarget();

    m_PassedTime += DT;

    if (m_PassedTime > 2.f)
    {
        if (nullptr == GetTarget())
        {
            ChangeState(BLADEKNIGHT_STATE::Wait);
        }
        else
        {
            if (m_bTraceUnit)
            {
                ChangeState(BLADEKNIGHT_STATE::FindWaitSub);
            }
            else
            {
                int RandomInt = GetRandomInt(1, 4);

                if (1 == RandomInt)
                {
                    ChangeState(BLADEKNIGHT_STATE::AttackStart);
                }
                else if (2 == RandomInt)
                {
                    ChangeState(BLADEKNIGHT_STATE::ThrustStart);
                }
                else if (3 == RandomInt)
                {
                    ChangeState(BLADEKNIGHT_STATE::TornadoAttackCharge);
                }
                else if (4 == RandomInt)
                {
                    ChangeState(BLADEKNIGHT_STATE::TornadoAttackCharge2);
                }
            }
        }
    }

    if (!IsGround())
    {
        ChangeState(BLADEKNIGHT_STATE::Fall);
    }
}

void CBladeKnightScript::FindWaitSub()
{
    RigidbodyMove();
    RotatingToTarget();

    if (nullptr == GetTarget())
    {
        ChangeState(BLADEKNIGHT_STATE::FindWait);
    }
    else
    {
        Vec3 dist = Transform()->GetWorldPos() - GetTarget()->Transform()->GetWorldPos();
        if (dist.Length() <= m_AttackRange)
        {
            int RandomInt = GetRandomInt(1, 4);

            if (1 == RandomInt)
            {
                ChangeState(BLADEKNIGHT_STATE::AttackStart);
            }
            else if (2 == RandomInt)
            {
                ChangeState(BLADEKNIGHT_STATE::ThrustStart);
            }
            else if (3 == RandomInt)
            {
                ChangeState(BLADEKNIGHT_STATE::TornadoAttackCharge);
            }
            else if (4 == RandomInt)
            {
                ChangeState(BLADEKNIGHT_STATE::TornadoAttackCharge2);
            }
        }
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
    if (CHECK_ANIMFRM(GetOwner(), 8) && !m_bStepFlag)
    {
        Rigidbody()->AddForce(-Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_StepPower * 0.8f, ForceMode::Impulse);
        m_bStepFlag = true;
    }
    else if (CHECK_ANIMFRM(GetOwner(), 24))
    {
        m_bStepFlag = false;
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    else if (CHECK_ANIMFRM(GetOwner(), 25) && !m_bStepFlag)
    {
        Rigidbody()->AddForce(-Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_StepPower * 0.8f, ForceMode::Impulse);
        m_bStepFlag = true;
    }

    if (Animator()->IsFinish())
    {
        m_bStepFlag = false;
        ChangeState(BLADEKNIGHT_STATE::FindWait);
    }
}

void CBladeKnightScript::Thrust()
{
    if (Animator()->IsFinish())
    {
        ChangeState(BLADEKNIGHT_STATE::ThrustLoop);
    }
}

void CBladeKnightScript::ThrustEnd()
{
    if (Animator()->IsFinish())
    {
        ChangeState(BLADEKNIGHT_STATE::FindWait);
    }
}

void CBladeKnightScript::ThrustLoop()
{
    m_PassedTime += DT;

    if (m_PassedTime > 0.3f)
    {
        ChangeState(BLADEKNIGHT_STATE::ThrustEnd);
    }
}

void CBladeKnightScript::ThrustStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(BLADEKNIGHT_STATE::ThrustStartWait);
    }
}

void CBladeKnightScript::ThrustStartWait()
{
    RotatingToTarget();

    m_PassedTime += DT;

    if (m_PassedTime > 1.f)
    {
        ChangeState(BLADEKNIGHT_STATE::Thrust);
    }
}

void CBladeKnightScript::ThrustWait()
{
    // Not Use
}

void CBladeKnightScript::TornadoAttack()
{
    if (Animator()->IsFinish())
    {
        ChangeState(BLADEKNIGHT_STATE::FindWait);
    }
}

void CBladeKnightScript::TornadoAttackCharge()
{
    if (Animator()->IsFinish())
    {
        ChangeState(BLADEKNIGHT_STATE::TornadoAttack);
    }
}

void CBladeKnightScript::TornadoAttackCharge2()
{
    if (Animator()->IsFinish())
    {
        ChangeState(BLADEKNIGHT_STATE::TornadoAttack);
    }
}

void CBladeKnightScript::TornadoAttackChargeMax()
{
    // Not Use
}

void CBladeKnightScript::Wait()
{
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

UINT CBladeKnightScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);
    fwrite(&m_StepPower, 1, sizeof(float), _File);
    fwrite(&m_AttackRange, 1, sizeof(float), _File);
    fwrite(&m_bTraceUnit, 1, sizeof(bool), _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CBladeKnightScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);
    fread(&m_StepPower, 1, sizeof(float), _File);
    fread(&m_AttackRange, 1, sizeof(float), _File);
    fread(&m_bTraceUnit, 1, sizeof(bool), _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}
