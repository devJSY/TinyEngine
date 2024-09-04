#include "pch.h"
#include "CSpookStepScript.h"

#include "CPlayerMgr.h"

CSpookStepScript::CSpookStepScript()
    : CMonsterUnitScript(SPOOKSTEPSCRIPT)
    , m_eState(SpookStepState::Appear)
{
}

CSpookStepScript::CSpookStepScript(const CSpookStepScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_eState(SpookStepState::Appear)
{
}

CSpookStepScript::~CSpookStepScript()
{
}

void CSpookStepScript::begin()
{
    CMonsterUnitScript::begin();

    ChangeState(SpookStepState::Appear);
}

void CSpookStepScript::tick()
{
    CUnitScript::tick();

    CheckDamage();

    IsDead();

    FSM();

    if (SpookStepState::Eaten != m_eState && GetResistState())
    {
        ChangeState(SpookStepState::Eaten);
    }
}

UINT CSpookStepScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CSpookStepScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);

    return MemoryByte;
}

void CSpookStepScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (SpookStepState::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();

    Vec3 vDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();
    UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, vDir.Normalize(), GetCurInfo().ATK, 0.f, 0.f};
    UINT Layer = _OtherCollider->GetOwner()->GetLayerIdx();

    if (Layer == LAYER_PLAYER_TRIGGER && L"Body Collider" == pObj->GetName())
    {
        BodyAttackSound();
        pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo);
    }
}

void CSpookStepScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

/////////////////////////// FUNC //////////////////////////////////
// 1. EnterState
// 2. FSM
// 3. ExitState
// 4. ChangeState
// 5. CheckDamage

void CSpookStepScript::EnterState()
{
    switch (m_eState)
    {
    case SpookStepState::Appear: {
        Animator()->Play(ANIMPREFIX("Appear"), false);
    }
    break;
    case SpookStepState::Wait: {
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("Wait"), true, false, 1.5f);
    }
    break;
    case SpookStepState::Find: {
        FindSound();
        Animator()->Play(ANIMPREFIX("Find"), false, false, 1.5f);
    }
    break;
    case SpookStepState::Move: {
        Animator()->Play(ANIMPREFIX("Move"), true, false, 1.5f);
    }
    break;
    case SpookStepState::Fall: {
        Animator()->Play(ANIMPREFIX("Fall"));
    }
    break;
    case SpookStepState::Landing: {
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("Landing"), false);
    }
    break;
    case SpookStepState::Damage: {
        Vec3 vDir = GetOwner()->GetScript<CUnitScript>()->GetHitDir();
        vDir.y = 1.f;
        AttackKnockBack(vDir, 6.f);

        Animator()->Play(ANIMPREFIX("Damage"), false, false, 1.f);
    }
    break;
    case SpookStepState::Eaten: {
        Animator()->Play(ANIMPREFIX("Damage"));
    }
    break;
    case SpookStepState::Disappear: {
        Vec3 vDir = GetOwner()->GetScript<CUnitScript>()->GetHitDir();
        vDir.y = 1.5f;
        AttackKnockBack(vDir, 9.f);
        SpawnDeadSmokeEffect();
        Animator()->Play(ANIMPREFIX("Disappear"), false);
    }
    break;
    case SpookStepState::End:
        break;
    default:
        break;
    }
}

void CSpookStepScript::FSM()
{
    if (!IsGround())
    {
        ChangeState(SpookStepState::Fall);
    }

    switch (m_eState)
    {
    case SpookStepState::Appear: {
        Appear();
    }
    break;
    case SpookStepState::Wait: {
        Wait();
    }
    break;
    case SpookStepState::Find: {
        Find();
    }
    break;
    case SpookStepState::Move: {
        Move();
    }
    break;
    case SpookStepState::Fall: {
        Fall();
    }
    break;
    case SpookStepState::Landing: {
        Landing();
    }
    break;
    case SpookStepState::Damage: {
        Damage();
    }
    break;
    case SpookStepState::Disappear: {
        Disappear();
    }
    break;
    case SpookStepState::Eaten: {
        Eaten();
    }
    break;
    case SpookStepState::End:
        break;
    default:
        break;
    }
}

void CSpookStepScript::ExitState()
{
    switch (m_eState)
    {
    case SpookStepState::Appear:
        break;
    case SpookStepState::Wait:
        break;
    case SpookStepState::Find:
        break;
    case SpookStepState::Move:
        break;
    case SpookStepState::Fall:
        break;
    case SpookStepState::Landing:
        break;
    case SpookStepState::Damage:
        break;
    case SpookStepState::Disappear:
        break;
    case SpookStepState::Eaten: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case SpookStepState::End:
        break;
    default:
        break;
    }
}

void CSpookStepScript::ChangeState(SpookStepState _state)
{
    if (SpookStepState::Disappear == m_eState)
    {
        return;
    }

    ExitState();
    m_eState = _state;
    EnterState();
}

void CSpookStepScript::CheckDamage()
{
    if (IsGetDamage())
    {
        ChangeState(SpookStepState::Damage);
    }
}

void CSpookStepScript::IsDead()
{
    if (m_CurInfo.HP <= 0.1f)
    {
        ChangeState(SpookStepState::Disappear);
    }
}

///////////////////////////// FIND FSM ///////////////////////////////////////
// -> FSM STATES ->
// 1. Appear
// 2. Wait
// 3. Find
// 4. Move
// 5. Fall
// 6. Landing
// 7. Damage
// 8. Disappear

#pragma region APPEAR
void CSpookStepScript::Appear()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SpookStepState::Fall);
    }
}
#pragma endregion

#pragma region WAIT
void CSpookStepScript::Wait()
{
    if (IsGround())
    {
        if (nullptr != GetTarget())
        {
            Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, false);

            ChangeState(SpookStepState::Find);
        }
    }
    else
    {
        if (nullptr != GetTarget())
        {
            Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, false);

            ChangeState(SpookStepState::Find);
        }
    }
}
#pragma endregion

#pragma region FIND
void CSpookStepScript::Find()
{
    RotatingToTarget();

    if (Animator()->IsFinish())
    {
        ChangeState(SpookStepState::Move);
    }
}
#pragma endregion

#pragma region MOVE
void CSpookStepScript::Move()
{
    RotatingToTarget();
    Rigidbody()->SetVelocity(Transform()->GetWorldDir(DIR_TYPE::FRONT) * GetCurInfo().Speed * DT);

    if (nullptr == GetTarget())
    {
        ChangeState(SpookStepState::Wait);
    }
}
#pragma endregion

#pragma region FALL
void CSpookStepScript::Fall()
{
    if (IsGround())
    {
        ChangeState(SpookStepState::Landing);
    }
}
#pragma endregion

#pragma region LANDING
void CSpookStepScript::Landing()
{
    if (Animator()->IsFinish())
    {
        ChangeState(SpookStepState::Wait);
    }
}
#pragma endregion

#pragma region DAMAGE
void CSpookStepScript::Damage()
{
    if (GetCurInfo().HP <= 0.1f)
    {
        ChangeState(SpookStepState::Disappear);
    }
    else
    {
        Animator()->IsFinish() ? ChangeState(SpookStepState::Fall) : void();
    }
}
#pragma endregion

#pragma region DISAPPEAR
void CSpookStepScript::Disappear()
{
    if (Animator()->IsFinish())
    {
        SpawnDeadEffect(2);
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}
#pragma endregion

void CSpookStepScript::Eaten()
{
    if (!GetResistState())
    {
        ChangeState(SpookStepState::Fall);
    }
}