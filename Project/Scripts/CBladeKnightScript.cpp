#include "pch.h"
#include "CBladeKnightScript.h"
#include "CBladeKnightSwordScript.h"

#include "CPlayerMgr.h"

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

    if (nullptr != Rigidbody())
    {
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
    }

    SetSword();

    m_StepPower = 5.f;

    ChangeState(m_State);
}

void CBladeKnightScript::tick()
{
    CMonsterUnitScript::tick();

    if (IsGetDamage())
    {
        ChangeState(BLADEKNIGHT_STATE::Damage);
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
    case BLADEKNIGHT_STATE::Eaten:
        Eaten();
        break;
    }

    if (BLADEKNIGHT_STATE::Eaten != m_State && GetResistState())
    {
        ChangeState(BLADEKNIGHT_STATE::Eaten);
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
            SetSwordEnable(false);
            break;
        }
    }
}

void CBladeKnightScript::SetSwordEnable(bool _bEnable)
{
    if (nullptr != m_Sword && nullptr != m_Sword->BoxCollider())
    {
        m_Sword->BoxCollider()->SetEnabled(_bEnable);
    }
}

void CBladeKnightScript::EnterState()
{
    switch (m_State)
    {
    case BLADEKNIGHT_STATE::Attack: {
        GamePlayStatic::Play2DSound(L"sound\\wav\\CharaBladeKnight\\0000.wav", 1, MONSTER_EFFECTSOUND);
        Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_StepPower, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("Attack"), false, false, 1.5f);
        SetSwordEnable(true);
    }
    break;
    case BLADEKNIGHT_STATE::AttackStart: {
        Animator()->Play(ANIMPREFIX("AttackStart"), false);
        SetSwordEnable(true);
    }
    break;
    case BLADEKNIGHT_STATE::Damage: {

        SetSparkle(true);

        // 피격 방향으로 회전
        Vec3 ToTargetDir = -GetHitDir();
        ToTargetDir.y = 0.f; // Y축 고정
        ToTargetDir.Normalize();
        Transform()->Slerp(ToTargetDir, 1.f);

        // 피격 방향으로 Impulse
        Vec3 Impulse = GetHitDir();
        float fForce = 0.f;
        if (GetCurInfo().HP <= 0.1f)
        {
            SpawnDeadSmokeEffect();
            fForce = 6.f;
            Impulse.y = 1.5f;
        }
        else
        {
            fForce = 3.5f;
            Impulse.y = 1.f;
        }

        Rigidbody()->AddForce(Impulse.Normalize() * fForce, ForceMode::Impulse);

        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case BLADEKNIGHT_STATE::DoubleAttack: {
        GamePlayStatic::Play2DSound(L"sound\\wav\\CharaBladeKnight\\0000.wav", 1, MONSTER_EFFECTSOUND);
        Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_StepPower, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("DoubleAttack"), false, false, 1.5f);
        SetSwordEnable(true);
    }
    break;
    case BLADEKNIGHT_STATE::Fall: {
        Animator()->Play(ANIMPREFIX("Fall"), true);
    }
    break;
    case BLADEKNIGHT_STATE::Find: {
        FindSound();
        Animator()->Play(ANIMPREFIX("Find"), false, false, 1.f);
    }
    break;
    case BLADEKNIGHT_STATE::FindWait: {
        Rigidbody()->SetAngularVelocity(Vec3(0.f, 0.f, 0.f));
        Animator()->Play(ANIMPREFIX("FindWait"));
    }
    break;
    case BLADEKNIGHT_STATE::FindWaitSub: {
        Animator()->Play(ANIMPREFIX("FindWaitSub"));
    }
    break;
    case BLADEKNIGHT_STATE::Landing: {
        LandingSmokeEffect(Vec3(0.f, -20.f, 0.f));
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
        Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_StepPower * 1.2f, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("Thrust"), false);
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::Thrust);
            SetSwordEnable(true);
        }
    }
    break;
    case BLADEKNIGHT_STATE::ThrustEnd: {
        Animator()->Play(ANIMPREFIX("ThrustEnd"), false, false, 1.f);
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::ThrustEnd);
            SetSwordEnable(true);
        }
    }
    break;
    case BLADEKNIGHT_STATE::ThrustLoop: {
        Animator()->Play(ANIMPREFIX("ThrustLoop"));
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::ThrustLoop);
            SetSwordEnable(true);
        }
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStart: {
        Animator()->Play(ANIMPREFIX("ThrustStart"), false);
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::ThrustStart);
            SetSwordEnable(true);
        }
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStartWait: {
        Animator()->Play(ANIMPREFIX("ThrustStartWait"));
        if (nullptr != m_Sword)
        {
            m_Sword->ChangeState(BLADEKNIGHTSWORD_STATE::ThrustStartWait);
            SetSwordEnable(true);
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
        GamePlayStatic::Play2DSound(L"sound\\wav\\CharaBladeKnight\\0005.wav", 1, MONSTER_EFFECTSOUND);
        Animator()->Play(ANIMPREFIX("TornadoAttack"), false, false, 1.5f);
        SetSwordEnable(true);
    }
    break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge: {
        Animator()->Play(ANIMPREFIX("TornadoAttackCharge"), false);
        SetSwordEnable(true);
    }
    break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge2: {
        Animator()->Play(ANIMPREFIX("TornadoAttackCharge2"), false);
        SetSwordEnable(true);
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
    case BLADEKNIGHT_STATE::Eaten: {
        Animator()->Play(ANIMPREFIX("Damage"));
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
        SetSwordEnable(false);
    }
    break;
    case BLADEKNIGHT_STATE::AttackStart: {
        SetSwordEnable(false);
    }
    break;
    case BLADEKNIGHT_STATE::Damage: {
        if (m_CurInfo.HP <= 0.f)
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
            SpawnDeadEffect(2);
        }
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Rigidbody()->SetAngularVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case BLADEKNIGHT_STATE::DoubleAttack: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        SetSwordEnable(false);
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
        SetSwordEnable(false);
    }
    break;
    case BLADEKNIGHT_STATE::Thrust: {
        SetSwordEnable(false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustEnd: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        SetSwordEnable(false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustLoop: {
        SetSwordEnable(false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStart: {
        SetSwordEnable(false);
    }
    break;
    case BLADEKNIGHT_STATE::ThrustStartWait: {
        Rigidbody()->SetAngularVelocity(Vec3(0.f, 0.f, 0.f));
        SetSwordEnable(false);
    }
    break;
    // case BLADEKNIGHT_STATE::ThrustWait: {
    // }
    // break;
    case BLADEKNIGHT_STATE::TornadoAttack: {
        SetSwordEnable(false);
    }
    break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge: {
        SetSwordEnable(false);
    }
    break;
    case BLADEKNIGHT_STATE::TornadoAttackCharge2: {
        SetSwordEnable(false);
    }
    break;
    // case BLADEKNIGHT_STATE::TornadoAttackChargeMax: {
    // }
    // break;
    case BLADEKNIGHT_STATE::Wait: {
    }
    break;
    case BLADEKNIGHT_STATE::Eaten: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
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
        GamePlayStatic::Play2DSound(L"sound\\wav\\CharaBladeKnight\\0000.wav", 1, MONSTER_EFFECTSOUND);
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

void CBladeKnightScript::Eaten()
{
    if (!GetResistState())
    {
        ChangeState(BLADEKNIGHT_STATE::Wait);
    }
}

void CBladeKnightScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (BLADEKNIGHT_STATE::Eaten == m_State)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();
    UINT Layer = _OtherCollider->GetOwner()->GetLayerIdx();
    if (Layer == LAYER_PLAYER_TRIGGER && L"Body Collider" == pObj->GetName())
    {
        BodyAttackSound();
        Vec3 vDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();
        UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, vDir.Normalize(), GetCurInfo().ATK, 0.f, 0.f};
        pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo);
    }
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
