#include "pch.h"
#include "CNormalEnemyScript.h"

CNormalEnemyScript::CNormalEnemyScript()
    : CMonsterUnitScript(NORMALENEMYSCRIPT)
    , m_pTargetObject(nullptr)
    , m_eState(NORMALENEMY_STATE::Idle)
    , m_fPatrolTime(4.f)
    , m_fPatrolAccTime(0.f)
    , m_fPatrolDir{}
    , m_fMaxSpeed(0.f)
    , m_fSpeed(0.f)
    , m_fRushLerp(0.f)
    , m_fRushSpeedLerp(0.f)
    , m_bFirst(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxSpeed, "Rush Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rush Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushLerp, "Rush Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushSpeedLerp, "Rush Speed Lerp");
}

CNormalEnemyScript::CNormalEnemyScript(const CNormalEnemyScript& _Origin)
    : CMonsterUnitScript(_Origin)
    , m_pTargetObject(nullptr)
    , m_eState(NORMALENEMY_STATE::Idle)
    , m_fPatrolTime(_Origin.m_fPatrolTime)
    , m_fPatrolAccTime(0.f)
    , m_fPatrolDir{}
    , m_fMaxSpeed(_Origin.m_fMaxSpeed)
    , m_fSpeed(_Origin.m_fSpeed)
    , m_fRushLerp(m_fRushLerp)
    , m_fRushSpeedLerp(m_fRushSpeedLerp)
    , m_bFirst(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxSpeed, "Rush Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rush Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushLerp, "Rush Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushSpeedLerp, "Rush Speed Lerp");
}

CNormalEnemyScript::~CNormalEnemyScript()
{
}

void CNormalEnemyScript::begin()
{
    ChangeState(NORMALENEMY_STATE::Idle);
}

void CNormalEnemyScript::tick()
{
    CUnitScript::tick();
    switch (m_eState)
    {
    case NORMALENEMY_STATE::Idle: {
        Idle();
    }
    break;
    case NORMALENEMY_STATE::Grooming: {
        Grooming();
    }
    break;
    case NORMALENEMY_STATE::Patrol: {
        Patrol();
    }
    break;
    case NORMALENEMY_STATE::Find: {
        Find();
    }
    break;
    case NORMALENEMY_STATE::Attack: {
        Attack();
    }
    break;
    case NORMALENEMY_STATE::AttackSuccessed: {
        SuccessedAttack();
    }
    break;
    case NORMALENEMY_STATE::AttackFailed: {
        FailedAttack();
    }
    break;
    case NORMALENEMY_STATE::Damage: {
        Damage();
    }
    break;
    case NORMALENEMY_STATE::AfterAttack: {
        AfterAttack();
    }
    break;
    case NORMALENEMY_STATE::Dead: {
        Dead();
    }
    break;
    case NORMALENEMY_STATE::Land: {
        Land();
    }
    break;
    case NORMALENEMY_STATE::End:
        break;
    default:
        break;
    }
}

void CNormalEnemyScript::ChangeState(NORMALENEMY_STATE _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(_state);

    string tmp = string("[State Change] : ") + to_string((int)_state);
    LOG(LOG_LEVEL::Log, tmp.c_str());
}

void CNormalEnemyScript::EnterState(NORMALENEMY_STATE _state)
{
    switch (_state)
    {
    case NORMALENEMY_STATE::Idle: {
        Animator()->Play(ANIMPREFIX(L"Wait"), false);
    }
    break;
    case NORMALENEMY_STATE::Grooming: {
        Animator()->Play(ANIMPREFIX(L"Grooming"));
    }
    break;
    case NORMALENEMY_STATE::Patrol: {
        Animator()->Play(ANIMPREFIX(L"Walk"), true, false, 1.5f);
    }
    break;
    case NORMALENEMY_STATE::Sleep: {
        Animator()->Play(ANIMPREFIX(L"Sleep"));
    }
    break;
    case NORMALENEMY_STATE::Find: {
        Animator()->Play(ANIMPREFIX(L"Find"), false);
    }
    break;
    case NORMALENEMY_STATE::Attack: {
        Animator()->Play(ANIMPREFIX(L"Run"));
    }
    break;
    case NORMALENEMY_STATE::AttackSuccessed: {
        Animator()->Play(ANIMPREFIX(L"Damage"), false, false, 1.5f);
        m_pTargetObject = nullptr;
    }
    break;
    case NORMALENEMY_STATE::AttackFailed: {
        Animator()->Play(ANIMPREFIX(L"Brake"), false, false, 1.5f);
    }
    break;
    case NORMALENEMY_STATE::AfterAttack: {
        Animator()->Play(ANIMPREFIX(L"LookAround"), false);
    }
    break;
    case NORMALENEMY_STATE::Damage: {
        Animator()->Play(ANIMPREFIX(L"Damage"), false, false, 1.5f);
    }
    break;
    case NORMALENEMY_STATE::Land: {
        Animator()->Play(ANIMPREFIX(L"Landing"), false);
    }
    break;
    case NORMALENEMY_STATE::Eaten: {
        m_vDamageDir.Normalize();
        Vec3 vUp = Vec3(0.f, 0.f, -1.f) == m_vDamageDir ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);
        Quat vQuat = Quat::LookRotation(m_vDamageDir, vUp);
        Transform()->SetWorldRotation(vQuat);
        Animator()->Play(ANIMPREFIX(L"Damage"), true, false, 1.5f);
    }
    break;
    case NORMALENEMY_STATE::End:
        break;
    default:
        break;
    }
}

void CNormalEnemyScript::ExitState(NORMALENEMY_STATE _state)
{
    switch (_state)
    {
    case NORMALENEMY_STATE::AttackSuccessed: {
        m_pTargetObject = nullptr;
        m_fSpeed = m_fMaxSpeed;
        m_bFirst = false;
    }
    break;
    case NORMALENEMY_STATE::AttackFailed: {
        m_pTargetObject = nullptr;
        m_fSpeed = m_fMaxSpeed;
    }
    break;
    case NORMALENEMY_STATE::Damage: {
        m_bFirst = false;
    }
    break;
    }
}

void CNormalEnemyScript::Idle()
{
    if (nullptr != GetTarget())
    {
        ChangeState(NORMALENEMY_STATE::Find);
    }
    else
    {
        if (Animator()->IsFinish())
        {
            ChangeState(RandomIdleState());
        }
    }
}

void CNormalEnemyScript::Grooming()
{
    if (nullptr != GetTarget())
    {
        ChangeState(NORMALENEMY_STATE::Find);
    }
    else
    {
        if (Animator()->IsFinish())
        {
            ChangeState(RandomIdleState());
        }
    }
}

void CNormalEnemyScript::Patrol()
{
    PatrolMove();

    // 발견 시
    nullptr != GetTarget() ? ChangeState(NORMALENEMY_STATE::Find) : void();
}

void CNormalEnemyScript::Find()
{
    m_pTargetObject = GetTarget();

    TransformRotate();

    if (Animator()->IsFinish())
    {
        ChangeState(NORMALENEMY_STATE::Attack);
    }
}

void CNormalEnemyScript::Attack()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;
    ApplyDir(vDir, true);

    m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if (m_fSpeed <= 3.f)
    {
        ChangeState(NORMALENEMY_STATE::AttackFailed);
    }
}

void CNormalEnemyScript::SuccessedAttack()
{
    if (!m_bFirst)
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
        vDir *= -1.f;
        vDir.y = 1.f;
        vDir = vDir.Normalize();
        vDir.y = 1.f;

        Rigidbody()->AddForce(vDir * 100.f, ForceMode::Impulse);
        m_bFirst = true;
    }

    if (Animator()->IsFinish())
    {
        m_bFirst = false;
        ChangeState(NORMALENEMY_STATE::Land);
    }
}

void CNormalEnemyScript::Land()
{
    if (Animator()->IsFinish() && Rigidbody()->GetVelocity().y >= -0.01f)
    {
        ChangeState(NORMALENEMY_STATE::AfterAttack);
    }
}

void CNormalEnemyScript::FailedAttack()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;

    m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if (m_fSpeed <= 0.3f)
    {
        ChangeState(NORMALENEMY_STATE::AfterAttack);
    }
}

void CNormalEnemyScript::AfterAttack()
{
    if (Animator()->IsFinish())
    {
        if (nullptr != GetTarget())
        {
            ChangeState(NORMALENEMY_STATE::Find);
        }
        else
        {
            if (Animator()->IsFinish())
            {
                ChangeState(RandomIdleState());
            }
        }
    }
}

void CNormalEnemyScript::Damage()
{
    if (!m_bFirst)
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        m_vDamageDir.Normalize();
        m_vDamageDir.y = 1.5f;
        Rigidbody()->AddForce(m_vDamageDir * 50.f, ForceMode::Impulse);
        m_bFirst = true;
    }

    if (Animator()->IsFinish())
    {
        if (nullptr != GetTarget())
        {
            ChangeState(NORMALENEMY_STATE::Find);
        }
        else
        {
            if (Animator()->IsFinish())
            {
                ChangeState(RandomIdleState());
            }
        }
        m_bFirst = false;
    }
}

void CNormalEnemyScript::Dead()
{
    if (Animator()->IsFinish())
        GamePlayStatic::DestroyGameObject(GetOwner());
}

NORMALENEMY_STATE CNormalEnemyScript::RandomIdleState()
{
    return NORMALENEMY_STATE(GetRandomInt(0, 2));
}

Vec3 CNormalEnemyScript::RandomPatrolDir()
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

void CNormalEnemyScript::ApplyDir(Vec3 _vFront, bool _flag)
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

void CNormalEnemyScript::PatrolMove()
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

void CNormalEnemyScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (NORMALENEMY_STATE::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();
    bool flag = false;

    UnitHit hit;
    ZeroMemory(&hit, sizeof(hit));
    /**********************
    | 1. Player ATK Hit
    ***********************/

    // 충돌한 오브젝트가 플레이어 공격인지 확인
    if (LAYER_PLAYERATK == pObj->GetLayerIdx())
    {
        flag = true;
        // TODO : 플레이어 공격 데미지 가지고 오기

        GetDamage(hit);
        ChangeState(NORMALENEMY_STATE::Damage);
        m_vDamageDir = pObj->GetParent()->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
    }

    /**********************
    | 2. Player Body Hit
    ***********************/
    // 충돌한 오브젝트가 PlayerBody인지 확인

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(NORMALENEMY_STATE::Eaten);
            m_vDamageDir = -pObj->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
            return;
        }

        flag = true;

        // 자기 자신의 데미지
        GetDamage(GetHitInfo());

        if (NORMALENEMY_STATE::AttackFailed == m_eState || NORMALENEMY_STATE::Attack == m_eState)
        {
            ChangeState(NORMALENEMY_STATE::AttackSuccessed);
        }
        else
        {
            ChangeState(NORMALENEMY_STATE::Damage);
            m_vDamageDir = pObj->GetParent()->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
        }
    }

    // 둘 중 하나라도 피격 되었다면 체력 확인
    if (flag)
    {
        if (GetCurInfo().HP - hit.Damage <= 0.f)
        {
            ChangeState(NORMALENEMY_STATE::Dead);
        }
    }
}

void CNormalEnemyScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이는게 끝난 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(NORMALENEMY_STATE::Idle);
        }
    }
}

Vec3 CNormalEnemyScript::TrackDir(Vec3 _vPos)
{
    return (m_pTargetObject->GetComponent<CTransform>()->GetLocalPos() - _vPos).Normalize();
}

void CNormalEnemyScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);

    fwrite(&m_fMaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    fwrite(&m_fRushLerp, sizeof(float), 1, _File);
    fwrite(&m_fRushSpeedLerp, sizeof(float), 1, _File);
}

void CNormalEnemyScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
    fread(&m_fMaxSpeed, sizeof(float), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);
    fread(&m_fRushLerp, sizeof(float), 1, _File);
    fread(&m_fRushSpeedLerp, sizeof(float), 1, _File);
}