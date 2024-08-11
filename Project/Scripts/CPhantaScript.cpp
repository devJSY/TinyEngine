#include "pch.h"
#include "CPhantaScript.h"

#include "CPlayerMgr.h"

CPhantaScript::CPhantaScript()
    : CMonsterUnitScript(SPOOKSTEPSCRIPT)
    , m_eState(PhantaState::Appear)
{
}

CPhantaScript::CPhantaScript(const CPhantaScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_eState(PhantaState::Appear)
{
}

CPhantaScript::~CPhantaScript()
{
}

void CPhantaScript::begin()
{
    CUnitScript::begin();

    ChangeState(PhantaState::Appear);
}

void CPhantaScript::tick()
{
    CMonsterUnitScript::tick();

    CheckDamage();

    FSM();
}

UINT CPhantaScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CPhantaScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);

    return MemoryByte;
}

void CPhantaScript::OnTriggerEnter(CCollider* _OtherCollider)
{
}

void CPhantaScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

/////////////////////////// FUNC //////////////////////////////////
// 1. EnterState
// 2. FSM
// 3. ExitState
// 4. ChangeState
// 5. CheckDamage

void CPhantaScript::EnterState()
{
    switch (m_eState)
    {
    case PhantaState::Appear: {
        Animator()->Play(ANIMPREFIX("Appear"), false);
    }
    break;
    case PhantaState::Wait: {
        Animator()->Play(ANIMPREFIX("Wait"));
    }
    break;
    case PhantaState::Find: {
        Animator()->Play(ANIMPREFIX("Find"), false);
    }
    break;
    case PhantaState::Attack: {
        Animator()->Play(ANIMPREFIX("Attack"));
    }
    break;
    case PhantaState::Brake: {
        Animator()->Play(ANIMPREFIX("Brake"), false);
    }
    break;
    case PhantaState::Damage: {
        SetSparkle(true);

        Transform()->SetDirection((PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize());

        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        Vec3 vHitDir = GetOwner()->GetScript<CUnitScript>()->GetHitDir();
        vHitDir.y = 1.5f;

        Rigidbody()->AddForce(vHitDir.Normalize() * 5.f, ForceMode::Impulse);

        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case PhantaState::Disappear: {
        Animator()->Play(ANIMPREFIX("Disappear"), false);
    }
    break;
    case PhantaState::End:
        break;
    default:
        break;
    }
}

void CPhantaScript::FSM()
{
    switch (m_eState)
    {
    case PhantaState::Appear: {
        Appear();
    }
    break;
    case PhantaState::Wait: {
        Wait();
    }
    break;
    case PhantaState::Find: {
        Find();
    }
    break;
    case PhantaState::Attack: {
        Attack();
    }
    break;
    case PhantaState::Brake: {
        Brake();
    }
    break;
    case PhantaState::Damage: {
        Damage();
    }
    break;
    case PhantaState::Disappear: {
        Disappear();
    }
    break;
    case PhantaState::End:
        break;
    default:
        break;
    }
}

void CPhantaScript::ExitState()
{
    switch (m_eState)
    {
    case PhantaState::Appear:
        break;
    case PhantaState::Wait:
        break;
    case PhantaState::Find:
        break;
    case PhantaState::Damage:
        break;
    case PhantaState::Disappear:
        break;
    case PhantaState::End:
        break;
    default:
        break;
    }
}

void CPhantaScript::ChangeState(PhantaState _state)
{
    ExitState();
    m_eState = _state;
    EnterState();
}

void CPhantaScript::CheckDamage()
{
    if (IsGetDamage())
    {
        ChangeState(PhantaState::Damage);
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

void CPhantaScript::Appear()
{
    if (Animator()->IsFinish())
    {
        ChangeState(PhantaState::Wait);
    }
}

void CPhantaScript::Wait()
{
    if (nullptr != GetTarget())
    {
        ChangeState(PhantaState::Find);
    }
}

void CPhantaScript::Find()
{
    RotatingToTarget();

    if (Animator()->IsFinish())
    {
        ChangeState(PhantaState::Attack);
    }
}

void CPhantaScript::Attack()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;
   /* ApplyDir(vDir, true);

    m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if (m_fSpeed <= 3.f)
    {
        ChangeState(TackleEnemyState::AttackAfter);
    }*/
}

void CPhantaScript::Brake()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;

    /*m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if (m_fSpeed <= 2.f)
    {
        ChangeState(TackleEnemyState::Wait);
    }*/
}

void CPhantaScript::Damage()
{
    if (GetCurInfo().HP <= 0.f)
    {
        ChangeState(PhantaState::Disappear);
    }

    Animator()->IsFinish() ? ChangeState(PhantaState::Wait) : void();
}

void CPhantaScript::Disappear()
{
    Animator()->IsFinish() ? GamePlayStatic::DestroyGameObject(GetOwner()) : void();
}