#include "pch.h"
#include "CTackleEnemyScript.h"

#include "CPlayerMgr.h"

CTackleEnemyScript::CTackleEnemyScript()
    : CMonsterUnitScript(TACKLEENEMYSCRIPT)
    , m_eState(TackleEnemyState::Idle)
    , m_fSpeed(10.f)
    , m_fMaxSpeed(10.f)
    , m_fRushLerp(0.9f)
    , m_fRushSpeedLerp(0.5f)
    , m_fAccTime(0.f)
    , m_fWaitTime(1.f)
    , m_fThreshHoldRushLerp(0.f)
    , m_vDamageDir{}
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxSpeed, "Rush Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rush Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushLerp, "Rush Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushSpeedLerp, "Rush Speed Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThreshHoldRushLerp, "Threshold Rush Lerp");
}

CTackleEnemyScript::CTackleEnemyScript(const CTackleEnemyScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_eState(TackleEnemyState::Idle)
    , m_fSpeed(Origin.m_fSpeed)
    , m_fMaxSpeed(Origin.m_fMaxSpeed)
    , m_fRushLerp(Origin.m_fRushLerp)
    , m_fRushSpeedLerp(Origin.m_fRushSpeedLerp)
    , m_fThreshHoldRushLerp(Origin.m_fThreshHoldRushLerp)
    , m_fAccTime(0.f)
    , m_fWaitTime(1.f)
    , m_vDamageDir{}
    , m_bFlag(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxSpeed, "Rush Max Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Rush Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushLerp, "Rush Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRushSpeedLerp, "Rush Speed Lerp");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fThreshHoldRushLerp, "Threshold Rush Lerp");
}

CTackleEnemyScript::~CTackleEnemyScript()
{
}

void CTackleEnemyScript::begin()
{
    CUnitScript::begin();

    ChangeState(TackleEnemyState::Idle);
}

void CTackleEnemyScript::tick()
{
    CMonsterUnitScript::tick();

    CheckDamage();

    FSM();
}

UINT CTackleEnemyScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);
    fwrite(&m_fMaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    fwrite(&m_fRushLerp, sizeof(float), 1, _File);
    fwrite(&m_fRushSpeedLerp, sizeof(float), 1, _File);
    fwrite(&m_fThreshHoldRushLerp, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
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
    fread(&m_fThreshHoldRushLerp, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

void CTackleEnemyScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (TackleEnemyState::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();
    bool flag = false;

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(TackleEnemyState::Eaten);
            m_vDamageDir = -pObj->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
            return;
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
            ChangeState(TackleEnemyState::Idle);
        }
    }
}

/////////////////////////// FUNC //////////////////////////////////
// 1. EnterState
// 2. FSM
// 3. ExitState
// 4. ChangeState
// 5. CheckDamage
// 6. TrackDir
// 7. ApplyDir

void CTackleEnemyScript::EnterState(TackleEnemyState _state)
{
    switch (_state)
    {
    case TackleEnemyState::Idle: {
        Animator()->Play(ANIMPREFIX("Wait"));
    }
    break;
    case TackleEnemyState::Find: {
        Animator()->Play(ANIMPREFIX("Find"), false);
    }
    break;
    case TackleEnemyState::AttackPrev: {
        Animator()->Play(ANIMPREFIX("RunStart"), false);
    }
    break;
    case TackleEnemyState::Attack: {
        Animator()->Play(ANIMPREFIX("Run"));
    }
    break;
    case TackleEnemyState::AttackAfter: {
        Animator()->Play(ANIMPREFIX("Brake"), false);
    }
    break;
    case TackleEnemyState::AttackAfter2: {
        Animator()->Play(ANIMPREFIX("Brake"), false);
    }
    break;
    case TackleEnemyState::Fall: {
        Animator()->Play(ANIMPREFIX("Fall"));
    }
    break;
    case TackleEnemyState::Landing: {
        Animator()->Play(ANIMPREFIX("Landing"),false);
    }
    break;
    case TackleEnemyState::Wait: {
        Animator()->Play(ANIMPREFIX("Wait"));
    }
    break;
    case TackleEnemyState::Damage: {
        SetSparkle(true);

        Transform()->SetDirection((PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize());

        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        Vec3 vHitDir = GetOwner()->GetScript<CUnitScript>()->GetHitDir();
        vHitDir.y = 1.5f;

        Rigidbody()->AddForce(vHitDir.Normalize() * 3.f, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case TackleEnemyState::Eaten: {
        m_vDamageDir.Normalize();
        Vec3 vUp = Vec3(0.f, 0.f, -1.f) == m_vDamageDir ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);
        Quat vQuat = Quat::LookRotation(-m_vDamageDir, vUp);
        Transform()->SetWorldRotation(vQuat);
        Animator()->Play(ANIMPREFIX("Damage"));
    }
    break;
    case TackleEnemyState::Death:
        break;
    default:
        break;
    }
}

void CTackleEnemyScript::FSM()
{
    switch (m_eState)
    {
    case TackleEnemyState::Idle: {
        Idle();
    }
    break;

    case TackleEnemyState::Find: {
        Find();
    }
    break;
    case TackleEnemyState::AttackPrev: {
        AttackPrev();
    }
    break;
    case TackleEnemyState::Attack: {
        Attack();
    }
    break;
    case TackleEnemyState::AttackAfter: {
        AttackAfter();
    }
    break;
    case TackleEnemyState::AttackAfter2: {
        AttackAfter2();
    }
    break;
    case TackleEnemyState::Wait: {
        Wait();
    }
    break;
    case TackleEnemyState::Fall: {
        Fall();
    }
    break;
    case TackleEnemyState::Landing: {
        Landing();
    }
    break;
    case TackleEnemyState::Damage: {
        Damage();
    }
    break;
    case TackleEnemyState::Eaten: {
        Eaten();
    }
    break;
    case TackleEnemyState::Death: {
        Death();
    }
    break;
    default:
        break;
    }
}

void CTackleEnemyScript::ExitState(TackleEnemyState _state)
{
    switch (_state)
    {
    case TackleEnemyState::Idle: {
    }
    break;
    case TackleEnemyState::Find: {
    }
    break;
    case TackleEnemyState::AttackPrev: {
    }
    break;
    case TackleEnemyState::Attack: {
    }
    break;
    case TackleEnemyState::AttackAfter: {
        m_fSpeed = m_fMaxSpeed;
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case TackleEnemyState::AttackAfter2: {
        m_fSpeed = m_fMaxSpeed;
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case TackleEnemyState::Damage: {
        m_bFlag = false;
    }
    break;
    case TackleEnemyState::Death:
        break;
    case TackleEnemyState::Wait: {
        m_fAccTime = 0.f;
    }
    break;
    default:
        break;
    }
}

void CTackleEnemyScript::ChangeState(TackleEnemyState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CTackleEnemyScript::CheckDamage()
{
    if (IsGetDamage())
    {
        ChangeState(TackleEnemyState::Damage);
    }
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
        ChangeState(TackleEnemyState::AttackAfter2);
        return;
    }

    if (_flag)
        _vTrackQuat = Quat::Slerp(_vOriginQuat, _vTrackQuat, m_fRushLerp * DT);

    Transform()->SetWorldRotation(_vTrackQuat);
}

Vec3 CTackleEnemyScript::TrackDir(Vec3 _vPos)
{
    return (PLAYER->GetComponent<CTransform>()->GetLocalPos() - _vPos).Normalize();
}

///////////////////////////// FIND FSM ///////////////////////////////////////
// -> FSM STATES ->
// 1. Idle
// 2. Find
// 3. AttackPrev
// 4. Attack
// 5. AttackAfter
// 6. AttackAfter2
// 7. Fall
// 8. Ladning
// 9. Damage
// 10. Eaten
// 11. Wait
// 12. Death

#pragma region IDLE
void CTackleEnemyScript::Idle()
{
    nullptr != GetTarget() ? ChangeState(TackleEnemyState::Find) : void();
}
#pragma endregion

#pragma region FIND
void CTackleEnemyScript::Find()
{
    Animator()->IsFinish() ? ChangeState(TackleEnemyState::AttackPrev) : void();
}
#pragma endregion

#pragma region ATTACKPREV
void CTackleEnemyScript::AttackPrev()
{
    RotatingToTarget();
    Animator()->IsFinish() ? ChangeState(TackleEnemyState::Attack) : void();
}
#pragma endregion

#pragma region ATTACK
void CTackleEnemyScript::Attack()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;
    ApplyDir(vDir, true);

    m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if (m_fSpeed <= 3.f)
    {
        ChangeState(TackleEnemyState::AttackAfter);
    }
}
#pragma endregion

#pragma region ATTACKAFTER
void CTackleEnemyScript::AttackAfter()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;

    m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if (m_fSpeed <= 2.f)
    {
        ChangeState(TackleEnemyState::Wait);
    }
}
#pragma endregion

#pragma region ATTACKAFTER2
void CTackleEnemyScript::AttackAfter2()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;

    m_fSpeed = Lerp(m_fSpeed, 0.f, 6.f * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if (m_fSpeed <= 2.f)
    {
        ChangeState(TackleEnemyState::Wait);
    }
}
#pragma endregion

#pragma region FALL
void CTackleEnemyScript::Fall()
{
    if (IsGround())
    {
        ChangeState(TackleEnemyState::Landing);
    }
}
#pragma endregion

#pragma region LADNING
void CTackleEnemyScript::Landing()
{
    if (Animator()->IsFinish())
    {
        ChangeState(TackleEnemyState::Idle);
    }
}
#pragma endregion

#pragma region DAMAGE
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

    if (GetCurInfo().HP <= 0.f)
    {
        ChangeState(TackleEnemyState::Death);
    }

    Animator()->IsFinish() ? ChangeState(TackleEnemyState::Fall) : void();
}
#pragma endregion

#pragma region EATEN
void CTackleEnemyScript::Eaten()
{
    Rigidbody()->AddForce(m_vDamageDir * 40.f, ForceMode::Force);
}
#pragma endregion

#pragma region WAIT
void CTackleEnemyScript::Wait()
{
    m_fAccTime += DT;
    if (m_fAccTime >= m_fWaitTime)
    {
        ChangeState(TackleEnemyState::Idle);
    }
}
#pragma endregion

#pragma region DEATH
void CTackleEnemyScript::Death()
{
    Animator()->IsFinish() ? GamePlayStatic::DestroyGameObject(GetOwner()) : void();
}
#pragma endregion