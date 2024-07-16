#include "pch.h"
#include "CTackleEnemyScript.h"

CTackleEnemyScript::CTackleEnemyScript()
    : CMonsterUnitScript(TACKLEENEMYSCRIPT)
    , m_fSpeed(10.f)
    , m_fMaxSpeed(10.f)
    , m_fRushLerp(0.9f)
    , m_fRushSpeedLerp(0.5f)
    , m_fPatrolTime(4.f)
    , m_fPatrolAccTime(0.f)
    , m_pTargetObject(nullptr)
{
}

CTackleEnemyScript::~CTackleEnemyScript()
{
}

void CTackleEnemyScript::begin()
{
    ChangeState(TACKLEENEMY_STATE::Idle);
}

void CTackleEnemyScript::tick()
{
    switch (m_eState)
    {
    case TACKLEENEMY_STATE::Idle: {
        Idle();
    }
    break;
    case TACKLEENEMY_STATE::Patrol: {
        Patrol();
    }
    break;
    case TACKLEENEMY_STATE::Find: {
        Find();
    }
    break;
    case TACKLEENEMY_STATE::AttackPrev: {
        AttackPrev();
    }
    break;
    case TACKLEENEMY_STATE::Attack: {
        Attack();
    }
    break;
    case TACKLEENEMY_STATE::AttackAfter: {
        AttackAfter();
    }
    break;
    case TACKLEENEMY_STATE::Damage: {
        Damage();
    }
    break;
    case TACKLEENEMY_STATE::Death: {
        Death();
    }
    break;
    default:
        break;
    }
}

void CTackleEnemyScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CTackleEnemyScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}

void CTackleEnemyScript::ChangeState(TACKLEENEMY_STATE _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CTackleEnemyScript::EnterState(TACKLEENEMY_STATE _state)
{
    switch (_state)
    {
    case TACKLEENEMY_STATE::Idle: {
        Animator()->Play(ANIMPREFIX(L"Wait"));
    }
    break;
    case TACKLEENEMY_STATE::Patrol: {
        Animator()->Play(ANIMPREFIX(L"Walk"));
    }
    break;
    case TACKLEENEMY_STATE::Find: {
        Animator()->Play(ANIMPREFIX(L"Find"));
    }
    break;
    case TACKLEENEMY_STATE::AttackPrev: {
        Animator()->Play(ANIMPREFIX(L"RunStart"), false);
    }
    break;
    case TACKLEENEMY_STATE::Attack: {
        Animator()->Play(ANIMPREFIX(L"Run"));
    }
    break;
    case TACKLEENEMY_STATE::AttackAfter: {
        Animator()->Play(ANIMPREFIX(L"Brake"), false);
    }
    break;
    case TACKLEENEMY_STATE::Damage: {
        Animator()->Play(ANIMPREFIX(L"Damage"), false);
    }
    break;
    case TACKLEENEMY_STATE::Death:
        break;
    default:
        break;
    }
}

void CTackleEnemyScript::ExitState(TACKLEENEMY_STATE _state)
{
    switch (_state)
    {
    case TACKLEENEMY_STATE::Idle: {
    }
    break;
    case TACKLEENEMY_STATE::Patrol: {
    }
    break;
    case TACKLEENEMY_STATE::Find: {
    }
    break;
    case TACKLEENEMY_STATE::AttackPrev: {
    }
    break;
    case TACKLEENEMY_STATE::Attack: {
        m_pTargetObject = nullptr;
    }
    break;
    case TACKLEENEMY_STATE::AttackAfter: {
        m_fSpeed = m_fMaxSpeed;
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case TACKLEENEMY_STATE::Damage: {
        Animator()->Play(ANIMPREFIX(L"Damage"), false);
    }
    break;
    case TACKLEENEMY_STATE::Death:
        break;
    default:
        break;
    }
}

void CTackleEnemyScript::Idle()
{
    nullptr != GetTarget() ? ChangeState(TACKLEENEMY_STATE::Find) : void();
}

void CTackleEnemyScript::Patrol()
{
    PatrolMove();

    // 발견 시
    nullptr != GetTarget() ? ChangeState(TACKLEENEMY_STATE::Find) : void();
}

void CTackleEnemyScript::Find()
{
    Animator()->IsFinish() ? ChangeState(TACKLEENEMY_STATE::AttackPrev) : void();
}

void CTackleEnemyScript::AttackPrev()
{
    Animator()->IsFinish() ? ChangeState(TACKLEENEMY_STATE::Attack) : void();
}

void CTackleEnemyScript::Attack()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;
    ApplyDir(vDir, true);

    m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if (m_fSpeed <= 3.f)
    {
        ChangeState(TACKLEENEMY_STATE::AttackAfter);
    }
}

void CTackleEnemyScript::AttackAfter()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;

    m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if (m_fSpeed <= 0.3f)
    {
        ChangeState(RandomIdleState());
    }
}

void CTackleEnemyScript::Damage()
{
    Animator()->IsFinish() ? ChangeState(TACKLEENEMY_STATE::Idle) : void();
}

void CTackleEnemyScript::Death()
{
    Animator()->IsFinish() ? GamePlayStatic::DestroyGameObject(GetOwner()) : void();
}

TACKLEENEMY_STATE CTackleEnemyScript::RandomIdleState()
{
    return TACKLEENEMY_STATE::Patrol; // TACKLEENEMY_STATE(GetRandomInt(0, 1));
}

Vec3 CTackleEnemyScript::RandomPatrolDir()
{
    PATROLDIR dirCount = PATROLDIR(GetRandomInt(0, 7));
    Vec3 dir = {};

    switch (dirCount)
    {
    case PATROLDIR::Up: {
        dir = Vec3(0.f, 0.f, 1.f);
    }
    break;
    case PATROLDIR::Down: {
        dir = Vec3(0.f, 0.f, -1.f);
    }
    break;
    case PATROLDIR::Right: {
        dir = Vec3(1.f, 0.f, 0.f);
    }
    break;
    case PATROLDIR::Left: {
        dir = Vec3(-1.f, 0.f, 0.f);
    }
    break;
    case PATROLDIR::UpLeft: {
        dir = Vec3(-1.f, 0.f, 1.f);
    }
    break;
    case PATROLDIR::UpRight: {
        dir = Vec3(1.f, 0.f, 1.f);
    }
    break;
    case PATROLDIR::DownLeft: {
        dir = Vec3(-1.f, 0.f, -1.f);
    }
    break;
    case PATROLDIR::DownRight: {
        dir = Vec3(1.f, 0.f, -1.f);
    }
    break;
    case PATROLDIR::END:
        break;
    default:
        break;
    }

    return dir.Normalize();
}

void CTackleEnemyScript::ApplyDir(Vec3 _vFront, bool _flag)
{
    Vec3 _vPos = Transform()->GetLocalPos();

    // Caculate Quaternion Tracking Dir
    Quat _vOriginQuat = Transform()->GetWorldQuaternion();
    Vec3 _vTrackingDir = TrackDir(_vPos);
    _vTrackingDir.y = 0.f;

    // Quternion Fix
    Vec3 _vUp = Vec3(0.f, 0.f, -1.f) == _vFront ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    Quat _vTrackQuat = Quat::LookRotation(-_vTrackingDir, _vUp);

    if (_flag)
        _vTrackQuat = Quat::Slerp(_vOriginQuat, _vTrackQuat, m_fRushLerp * DT);

    Transform()->SetWorldRotation(_vTrackQuat);
}

void CTackleEnemyScript::PatrolMove()
{
    // Patrol 방향은 매 패트롤 시간마다 바뀜
    Vec3 vUp = {};
    Vec3 vPos = Transform()->GetLocalPos();
    float fSpeed = GetCurInfo().Speed;
    CTransform* pTr = Transform();

    m_fPatrolAccTime += DT;

    Vec3 _vPatrolDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    if (m_fPatrolTime <= m_fPatrolAccTime)
    {
        Vec3 vDir = RandomPatrolDir();
        Vec3 vUP = Vec3(0.f, 1.f, 0.f);

        if (Vec3(0.f, 0.f, -1.f) == vDir)
        {
            vUP = Vec3(0.f, -1.f, 0.f);
        }

        Quat qPatrolQuat = Quat::LookRotation(-vDir, vUP);
        pTr->SetWorldRotation(qPatrolQuat);
        m_fPatrolAccTime = 0.f;
    }

    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Rigidbody()->SetVelocity(vDir * fSpeed);
}

Vec3 CTackleEnemyScript::TrackDir(Vec3 _vPos)
{
    return (m_pTargetObject->GetComponent<CTransform>()->GetLocalPos() - _vPos).Normalize();
}