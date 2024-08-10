#include "pch.h"
#include "CNormalEnemyScript.h"

#include "CPlayerMgr.h"

CNormalEnemyScript::CNormalEnemyScript()
    : CMonsterUnitScript(NORMALENEMYSCRIPT)
    , m_eState(NORMALENEMY_STATE::Idle)
    , m_vPatrolDir{}
    , m_vDamageDir{}
    , m_vCenterPoint{}
    , m_fMaxSpeed(0.f)
    , m_fSpeed(0.f)
    , m_fRushLerp(0.f)
    , m_fRushSpeedLerp(0.f)
    , m_fThreshHoldRushSpeedLerp(0.f)
    , m_bEnter(true)
    , m_bFirst(false)
    , m_bCirclePatrol(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxSpeed, "Rush Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rush Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushLerp, "Rush Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushSpeedLerp, "Rush Speed Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThreshHoldRushSpeedLerp, "ThreshHold Rush Speed Lerp");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCirclePatrol, "Circle Patrol");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPoint, "Center Point");
}

CNormalEnemyScript::CNormalEnemyScript(const CNormalEnemyScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_eState(NORMALENEMY_STATE::Idle)
    , m_vPatrolDir{}
    , m_vDamageDir{}
    , m_vCenterPoint{}
    , m_fMaxSpeed(Origin.m_fMaxSpeed)
    , m_fSpeed(Origin.m_fSpeed)
    , m_fRushLerp(Origin.m_fRushLerp)
    , m_fRushSpeedLerp(Origin.m_fRushSpeedLerp)
    , m_fThreshHoldRushSpeedLerp(Origin.m_fThreshHoldRushSpeedLerp)
    , m_bEnter(true)
    , m_bFirst(false)
    , m_bCirclePatrol(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxSpeed, "Rush Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rush Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushLerp, "Rush Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushSpeedLerp, "Rush Speed Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThreshHoldRushSpeedLerp, "ThreshHold Rush Speed Lerp");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCirclePatrol, "Circle Patrol");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPoint, "Center Point");
}

CNormalEnemyScript::~CNormalEnemyScript()
{
}

void CNormalEnemyScript::begin()
{
    m_bCirclePatrol == true ? ChangeState(NORMALENEMY_STATE::Patrol) : ChangeState(NORMALENEMY_STATE::Idle);
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
    case NORMALENEMY_STATE::Fall: {
        Fall();
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
    EnterState(m_eState);

    string tmp = string("[State Change] : ") + to_string((int)_state);
    LOG(LOG_LEVEL::Log, tmp.c_str());
}

void CNormalEnemyScript::EnterState(NORMALENEMY_STATE _state)
{
    switch (_state)
    {
    case NORMALENEMY_STATE::Idle: {
        Animator()->Play(ANIMPREFIX("Wait"), false);
    }
    break;
    case NORMALENEMY_STATE::Grooming: {
        Animator()->Play(ANIMPREFIX("Grooming"));
    }
    break;
    case NORMALENEMY_STATE::Patrol: {
        Animator()->Play(ANIMPREFIX("Walk"), true, false, 1.5f);
    }
    break;
    case NORMALENEMY_STATE::Sleep: {
        Animator()->Play(ANIMPREFIX("Sleep"));
    }
    break;
    case NORMALENEMY_STATE::Find: {
        Animator()->Play(ANIMPREFIX("Find"), false);
    }
    break;
    case NORMALENEMY_STATE::Attack: {
        Animator()->Play(ANIMPREFIX("Run"), true, false, 1.5f);
    }
    break;
    case NORMALENEMY_STATE::AttackSuccessed: {
        Animator()->Play(ANIMPREFIX("Damage"), false, false, 1.5f);
    }
    break;
    case NORMALENEMY_STATE::AttackFailed: {
        Animator()->Play(ANIMPREFIX("Brake"), false, false, 1.5f);
    }
    break;
    case NORMALENEMY_STATE::AfterAttack: {
        Animator()->Play(ANIMPREFIX("LookAround"), false);
    }
    break;
    case NORMALENEMY_STATE::Damage: {
        Animator()->Play(ANIMPREFIX("Damage"), false, false, 1.5f);
    }
    break;
    case NORMALENEMY_STATE::Fall: {
        Animator()->Play(ANIMPREFIX("Fall"));
    }
    break;
    case NORMALENEMY_STATE::Land: {
        Animator()->Play(ANIMPREFIX("Landing"), false);
    }
    break;
    case NORMALENEMY_STATE::Eaten: {
        m_vDamageDir.Normalize();
        Vec3 vUp = Vec3(0.f, 0.f, -1.f) == m_vDamageDir ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);
        Quat vQuat = Quat::LookRotation(m_vDamageDir, vUp);
        Transform()->SetWorldRotation(vQuat);
        Animator()->Play(ANIMPREFIX("Damage"), true, false, 1.5f);
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
        m_fSpeed = m_fMaxSpeed;
        m_bFirst = false;
    }
    break;
    case NORMALENEMY_STATE::AttackFailed: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
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
    if (IsGround())
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
    else
    {
        ChangeState(NORMALENEMY_STATE::Fall);
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
    if (IsGround())
    {
        PatrolMove();

        if (nullptr != GetTarget())
        {
            ChangeState(NORMALENEMY_STATE::Find);
        }
    }
    else
    {
        ChangeState(NORMALENEMY_STATE::Fall);
    }
}

void CNormalEnemyScript::Find()
{
    RotatingToTarget();

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
    if (Animator()->IsFinish())
    {
        m_bFirst = false;
        ChangeState(NORMALENEMY_STATE::Land);
    }
}

void CNormalEnemyScript::Fall()
{
    if (IsGround())
    {
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

    Animator()->GetCurFrameIdx();

    if (m_fSpeed <= 2.2f)
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
                if (m_bEnter && m_bCirclePatrol)
                {
                    ChangeState(NORMALENEMY_STATE::Patrol);
                    m_bEnter = false;
                }
                else
                {
                    ChangeState(RandomIdleState());
                }
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
    return NORMALENEMY_STATE(GetRandomInt(0, 1));
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

    float _vRadian = Quat::Angle(_vOriginQuat, _vTrackQuat);

    if (_vRadian >= (XM_2PI / 4.f) * 3.f)
    {
        _vRadian = XM_2PI - _vRadian;
    }
    else if (_vRadian >= XM_PI)
    {
        _vRadian = _vRadian - XM_PI;
    }

    // 90도 이상 틀어질 경우 lerp가 확도는걸 감안함
    if (_vRadian >= 1.5)
    {
        if (_flag)
        {
            _vTrackQuat = Quat::Slerp(_vOriginQuat, _vTrackQuat, m_fThreshHoldRushSpeedLerp * DT);
        }

        Transform()->SetWorldRotation(_vTrackQuat);
        return;
    }

    if (_flag)
        _vTrackQuat = Quat::Slerp(_vOriginQuat, _vTrackQuat, m_fRushLerp * DT);

    Transform()->SetWorldRotation(_vTrackQuat);
}

void CNormalEnemyScript::PatrolMove()
{
    Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 vPos = Transform()->GetWorldPos();
    Vec3 vUp = Vec3(0.f, 0.f, -1.f) == vFront ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    Vec3 vTemp = (m_vCenterPoint - vPos).Normalize();

    Vec3 vPatrolDir = vTemp.Cross(vUp);

    vPatrolDir = vPatrolDir.Normalize();

    vUp = vPatrolDir == Vec3(0.f, 0.f, -1.f) ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    Quat vPatrlQuat = Quat::LookRotation(vPatrolDir, vUp);

    Transform()->SetWorldRotation(vPatrlQuat);

    Rigidbody()->SetVelocity(vFront * GetCurInfo().Speed * DT);
}

Vec3 CNormalEnemyScript::TrackDir(Vec3 _vPos)
{
    return (PLAYER->GetComponent<CTransform>()->GetLocalPos() - _vPos).Normalize();
}

void CNormalEnemyScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (NORMALENEMY_STATE::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();
    bool flag = false;
    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(NORMALENEMY_STATE::Eaten);
            m_vDamageDir = -pObj->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
            return;
        }

        if (NORMALENEMY_STATE::AttackFailed == m_eState || NORMALENEMY_STATE::Attack == m_eState)
        {
            UnitHit hitInfo = {};
            L"Body Collider" == pObj->GetName() ? pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo) : void();
            ChangeState(NORMALENEMY_STATE::AttackSuccessed);
        }
        else
        {
            ChangeState(NORMALENEMY_STATE::Damage);
            m_vDamageDir = pObj->GetParent()->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
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

UINT CNormalEnemyScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);

    fwrite(&m_fMaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    fwrite(&m_fRushLerp, sizeof(float), 1, _File);
    fwrite(&m_fRushSpeedLerp, sizeof(float), 1, _File);
    fwrite(&m_fThreshHoldRushSpeedLerp, sizeof(float), 1, _File);
    fwrite(&m_bCirclePatrol, sizeof(bool), 1, _File);
    fwrite(&m_vCenterPoint, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}

UINT CNormalEnemyScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);

    fread(&m_fMaxSpeed, sizeof(float), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);
    fread(&m_fRushLerp, sizeof(float), 1, _File);
    fread(&m_fRushSpeedLerp, sizeof(float), 1, _File);
    fread(&m_fThreshHoldRushSpeedLerp, sizeof(float), 1, _File);
    fread(&m_bCirclePatrol, sizeof(bool), 1, _File);
    fread(&m_vCenterPoint, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}