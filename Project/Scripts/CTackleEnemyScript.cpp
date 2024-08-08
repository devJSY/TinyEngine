#include "pch.h"
#include "CTackleEnemyScript.h"

CTackleEnemyScript::CTackleEnemyScript()
    : CMonsterUnitScript(TACKLEENEMYSCRIPT)
    , m_eState(TACKLEENEMY_STATE::Idle)
    , m_fSpeed(10.f)
    , m_fMaxSpeed(10.f)
    , m_fRushLerp(0.9f)
    , m_fRushSpeedLerp(0.5f)
    , m_fPatrolTime(4.f)
    , m_fPatrolAccTime(0.f)
    , m_fThreshHoldRushLerp(0.f)
    , m_pTargetObject(nullptr)
    , m_vDamageDir{}
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxSpeed, "Rush Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rush Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushLerp, "Rush Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushSpeedLerp, "Rush Speed Lerp");
}

CTackleEnemyScript::CTackleEnemyScript(const CTackleEnemyScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_eState(TACKLEENEMY_STATE::Idle)
    , m_fSpeed(Origin.m_fSpeed)
    , m_fMaxSpeed(Origin.m_fMaxSpeed)
    , m_fRushLerp(Origin.m_fRushLerp)
    , m_fRushSpeedLerp(Origin.m_fRushSpeedLerp)
    , m_fThreshHoldRushLerp(Origin.m_fThreshHoldRushLerp)
    , m_fPatrolTime(Origin.m_fPatrolTime)
    , m_fPatrolAccTime(0.f)
    , m_pTargetObject(nullptr)
    , m_vDamageDir{}
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxSpeed, "Rush Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rush Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushLerp, "Rush Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushSpeedLerp, "Rush Speed Lerp");
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
    case TACKLEENEMY_STATE::Eaten: {
        Eaten();
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
        Animator()->Play(ANIMPREFIX("Wait"));
    }
    break;
    case TACKLEENEMY_STATE::Patrol: {
        Animator()->Play(ANIMPREFIX("Walk"));
    }
    break;
    case TACKLEENEMY_STATE::Find: {
        Animator()->Play(ANIMPREFIX("Find"), false);
    }
    break;
    case TACKLEENEMY_STATE::AttackPrev: {
        Animator()->Play(ANIMPREFIX("RunStart"), false);
    }
    break;
    case TACKLEENEMY_STATE::Attack: {
        Animator()->Play(ANIMPREFIX("Run"));
    }
    break;
    case TACKLEENEMY_STATE::AttackAfter: {
        Animator()->Play(ANIMPREFIX("Brake"), false);
    }
    break;
    case TACKLEENEMY_STATE::Damage: {
        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case TACKLEENEMY_STATE::Eaten: {
        m_vDamageDir.Normalize();
        Vec3 vUp = Vec3(0.f, 0.f, -1.f) == m_vDamageDir ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);
        Quat vQuat = Quat::LookRotation(-m_vDamageDir, vUp);
        Transform()->SetWorldRotation(vQuat);
        Animator()->Play(ANIMPREFIX("Damage"));
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
        m_bFlag = false;
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

void CTackleEnemyScript::Find()
{
    m_pTargetObject = GetTarget();
    Animator()->IsFinish() ? ChangeState(TACKLEENEMY_STATE::AttackPrev) : void();
}

void CTackleEnemyScript::AttackPrev()
{
    RotatingToTarget();
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
    if (!m_bFlag)
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        m_vDamageDir.Normalize();
        m_vDamageDir.y = 1.5f;
        Rigidbody()->AddForce(m_vDamageDir * 50.f, ForceMode::Impulse);
        m_bFlag = true;
    }

    Animator()->IsFinish() ? ChangeState(TACKLEENEMY_STATE::Idle) : void();
}

void CTackleEnemyScript::Eaten()
{
    Rigidbody()->AddForce(m_vDamageDir * 40.f, ForceMode::Force);
}

void CTackleEnemyScript::Death()
{
    Animator()->IsFinish() ? GamePlayStatic::DestroyGameObject(GetOwner()) : void();
}

TACKLEENEMY_STATE CTackleEnemyScript::RandomIdleState()
{
    return TACKLEENEMY_STATE(GetRandomInt(0, 1));
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

    float _vRadian = Quat::Angle(_vOriginQuat, _vTrackQuat);

    // 90도 이상 틀어질 경우 lerp가 확도는걸 감안함
    if (_vRadian >= 1.5)
    {
        if (_flag)
        {
            _vTrackQuat = Quat::Slerp(_vOriginQuat, _vTrackQuat, m_fThreshHoldRushLerp * DT);
        }

        Transform()->SetWorldRotation(_vTrackQuat);
        return;
    }

    if (_flag)
        _vTrackQuat = Quat::Slerp(_vOriginQuat, _vTrackQuat, m_fRushLerp * DT);

    Transform()->SetWorldRotation(_vTrackQuat);
}

Vec3 CTackleEnemyScript::TrackDir(Vec3 _vPos)
{
    return (m_pTargetObject->GetComponent<CTransform>()->GetLocalPos() - _vPos).Normalize();
}

UINT CTackleEnemyScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);
    fwrite(&m_fMaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    fwrite(&m_fRushLerp, sizeof(float), 1, _File);
    fwrite(&m_fRushSpeedLerp, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CTackleEnemyScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);
    fread(&m_fMaxSpeed, sizeof(float), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);
    fread(&m_fRushLerp, sizeof(float), 1, _File);
    fread(&m_fRushSpeedLerp, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

void CTackleEnemyScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (TACKLEENEMY_STATE::Eaten == m_eState)
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
        ChangeState(TACKLEENEMY_STATE::Damage);
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
            ChangeState(TACKLEENEMY_STATE::Eaten);
            m_vDamageDir = -pObj->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
            return;
        }
        UnitHit hitInfo = {};
        L"Body Collider" == pObj->GetName() ? pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo) : void();
    }

    // 둘 중 하나라도 피격 되었다면 체력 확인
    if (flag)
    {
        if (GetCurInfo().HP - hit.Damage <= 0.f)
        {
            ChangeState(TACKLEENEMY_STATE::Death);
        }
    }
}

void CTackleEnemyScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(TACKLEENEMY_STATE::Idle);
        }
    }
}