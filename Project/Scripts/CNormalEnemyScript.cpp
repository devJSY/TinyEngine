#include "pch.h"
#include "CNormalEnemyScript.h"

#include "CPlayerMgr.h"

CNormalEnemyScript::CNormalEnemyScript()
    : CMonsterUnitScript(NORMALENEMYSCRIPT)
    , m_eState(NormalEnemyState::Idle)
    , m_SnotBubble(nullptr)
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
    , m_bSleep(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bSleep, "Start Sleep");
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
    , m_eState(NormalEnemyState::Idle)
    , m_SnotBubble(nullptr)
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
    , m_bSleep(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bSleep, "Start Sleep");
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
    CMonsterUnitScript::begin();

    GetOwner()->MeshRender()->GetDynamicMaterial(0);

    SetSnotBubble();

    m_bSleep == true          ? ChangeState(NormalEnemyState::Sleep)
    : m_bCirclePatrol == true ? ChangeState(NormalEnemyState::Patrol)
                              : ChangeState(NormalEnemyState::Idle);

    SetInfo(UnitInfo{14.f, 14.f, 70.f, 7.f, 1.f, 5.f});
    m_fMaxSpeed = m_fSpeed = 12.f;
    m_fRushLerp = 0.8f;
    m_fRushSpeedLerp = 0.2f;
    m_fThreshHoldRushSpeedLerp = 0.1f;
}

void CNormalEnemyScript::tick()
{
    CMonsterUnitScript::tick();

    CheckDamage();

    FSM();

    if (NormalEnemyState::Eaten != m_eState && GetResistState())
    {
        ChangeState(NormalEnemyState::Eaten);
    }
}

void CNormalEnemyScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (NormalEnemyState::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();

    if (!PLAYER)
        return;

    Vec3 vDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();
    UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, vDir.Normalize(), GetCurInfo().ATK, 0.f, 0.f};
    UINT Layer = _OtherCollider->GetOwner()->GetLayerIdx();

    if (Layer == LAYER_PLAYER_TRIGGER && L"Body Collider" == pObj->GetName())
    {
        BodyAttackSound();
        pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo);
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
            ChangeState(NormalEnemyState::Idle);
        }
    }
}

UINT CNormalEnemyScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);

    fwrite(&m_bSleep, sizeof(bool), 1, _File);
    fwrite(&m_fMaxSpeed, sizeof(float), 1, _File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
    fwrite(&m_fRushLerp, sizeof(float), 1, _File);
    fwrite(&m_fRushSpeedLerp, sizeof(float), 1, _File);
    fwrite(&m_fThreshHoldRushSpeedLerp, sizeof(float), 1, _File);
    fwrite(&m_bCirclePatrol, sizeof(bool), 1, _File);
    fwrite(&m_vCenterPoint, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(bool);
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

    fread(&m_bSleep, sizeof(bool), 1, _File);
    fread(&m_fMaxSpeed, sizeof(float), 1, _File);
    fread(&m_fSpeed, sizeof(float), 1, _File);
    fread(&m_fRushLerp, sizeof(float), 1, _File);
    fread(&m_fRushSpeedLerp, sizeof(float), 1, _File);
    fread(&m_fThreshHoldRushSpeedLerp, sizeof(float), 1, _File);
    fread(&m_bCirclePatrol, sizeof(bool), 1, _File);
    fread(&m_vCenterPoint, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}

/////////////////////////// FUNC //////////////////////////////////
// 1. EnterState
// 2. FSM
// 3. ExitState
// 4. ChangeState
// 5. CheckDamage
// 6. RandomIdleState
// 7. TrackDir
// 8. ApplyDir
// 9. PatrolMove

void CNormalEnemyScript::SetSnotBubble()
{
    for (CGameObject* pChild : GetOwner()->GetChildObject())
    {
        if (L"SleepSnotBubble" == pChild->GetName())
        {
            m_SnotBubble = pChild;
            break;
        }
    }
}

void CNormalEnemyScript::EnterState(NormalEnemyState _state)
{
    if (_state != NormalEnemyState::Sleep)
    {
        if (m_SnotBubble)
            m_SnotBubble->SetActive(false);
    }

    switch (_state)
    {
    case NormalEnemyState::Idle: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png"));
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("Wait"), true, false, 1.5f);
    }
    break;
    case NormalEnemyState::Grooming: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png"));
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("Grooming"), true, false, 1.5f);
    }
    break;
    case NormalEnemyState::Patrol: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png"));
        Animator()->Play(ANIMPREFIX("Walk"), true, false, 1.5f);
    }
    break;
    case NormalEnemyState::Sleep: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.02.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.02.png"));

        m_SnotBubble->SetActive(true);

        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("Sleep"), true, false, 1.5f);
    }
    break;
    case NormalEnemyState::Find: {
        FindSound();
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png"));
        Animator()->Play(ANIMPREFIX("Find"), false, false, 1.5f);
    }
    break;
    case NormalEnemyState::Attack: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.04.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.04.png"));
        Animator()->Play(ANIMPREFIX("Run"), true, false, 1.5f);
    }
    break;
    case NormalEnemyState::Brake: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png"));
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("Brake"), false);
    }
    break;
    case NormalEnemyState::AfterAttack: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png"));
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("LookAround"), false);
    }
    break;
    case NormalEnemyState::Damage: {

        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png"));
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, false);

        SetSparkle(true);

        Vec3 vFollowDir = (PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
        vFollowDir.y = 0.f;

        Transform()->SetDirection(vFollowDir);

        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        Vec3 vHitDir = GetOwner()->GetScript<CUnitScript>()->GetHitDir();
        float fForce = 0.f;
        if (GetCurInfo().HP <= 0.1f)
        {
            fForce = 14.f;
            vHitDir.y = 1.5f;
        }
        else
        {
            fForce = 9.f;
            vHitDir.y = 1.2f;
        }

        Rigidbody()->AddForce(vHitDir.Normalize() * fForce, ForceMode::Impulse);

        Animator()->Play(ANIMPREFIX("Damage"), false, false, 2.f);
    }
    break;
    case NormalEnemyState::Fall: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png"));
        Animator()->Play(ANIMPREFIX("Fall"));
    }
    break;
    case NormalEnemyState::Land: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png"));
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);

        Animator()->Play(ANIMPREFIX("Landing"), false);
    }
    break;
    case NormalEnemyState::Eaten: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png"));
        Animator()->Play(ANIMPREFIX("Damage"), true, false, 1.5f);
    }
    break;
    case NormalEnemyState::Death: {
        SpawnDeadSmokeEffect();
    }
    break;
    case NormalEnemyState::End:
        break;
    default:
        break;
    }
}

void CNormalEnemyScript::FSM()
{
    switch (m_eState)
    {
    case NormalEnemyState::Idle: {
        Idle();
    }
    break;
    case NormalEnemyState::Sleep: {
        Sleep();
    }
    break;
    case NormalEnemyState::Grooming: {
        Grooming();
    }
    break;
    case NormalEnemyState::Patrol: {
        Patrol();
    }
    break;
    case NormalEnemyState::Find: {
        Find();
    }
    break;
    case NormalEnemyState::Attack: {
        Attack();
    }
    break;
    case NormalEnemyState::Brake: {
        Brake();
    }
    break;
    case NormalEnemyState::Damage: {
        Damage();
    }
    break;
    case NormalEnemyState::AfterAttack: {
        AfterAttack();
    }
    break;
    case NormalEnemyState::Eaten: {
        Eaten();
    }
    break;
    case NormalEnemyState::Death: {
        Death();
    }
    break;
    case NormalEnemyState::Land: {
        Land();
    }
    break;
    case NormalEnemyState::Fall: {
        Fall();
    }
    break;
    case NormalEnemyState::End:
        break;
    default:
        break;
    }
}

void CNormalEnemyScript::ExitState(NormalEnemyState _state)
{
    switch (_state)
    {
    case NormalEnemyState::Idle: {
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, false);
    }
    break;
    case NormalEnemyState::Sleep: {
        m_SnotBubble->SetActive(false);
    }
    break;
    case NormalEnemyState::Brake: {
        m_fSpeed = m_fMaxSpeed;
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case NormalEnemyState::Damage: {
        m_fSpeed = m_fMaxSpeed;
        m_bFirst = false;
    }
    break;
    case NormalEnemyState::Eaten: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    }
}

void CNormalEnemyScript::ChangeState(NormalEnemyState _state)
{
    if (m_eState == NormalEnemyState::Death)
        return;

    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CNormalEnemyScript::CheckDamage()
{
    if (IsGetDamage())
    {
        ChangeState(NormalEnemyState::Damage);
    }
}

NormalEnemyState CNormalEnemyScript::RandomIdleState()
{
    return NormalEnemyState(GetRandomInt(0, 1));
}

Vec3 CNormalEnemyScript::TrackDir(Vec3 _vPos)
{
    return (PLAYER->GetComponent<CTransform>()->GetLocalPos() - _vPos).Normalize();
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

    float _vRadian = Transform()->GetWorldDir(DIR_TYPE::FRONT).Dot((PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize());

    // 90도 이상 틀어질 경우 lerp가 확도는걸 감안함
    if (_vRadian <= 0.2f)
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

    vTemp.y = 0.f;

    Vec3 vPatrolDir = vTemp.Cross(vUp);

    vPatrolDir = vPatrolDir.Normalize();

    vUp = vPatrolDir == Vec3(0.f, 0.f, -1.f) ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    Quat vPatrlQuat = Quat::LookRotation(vPatrolDir, vUp);

    Transform()->SetWorldRotation(vPatrlQuat);

    Rigidbody()->SetVelocity(vFront * GetCurInfo().Speed * DT + vTemp * 3.5f * DT + Vec3(0.f, -9.81f, 0.f));
}

///////////////////////////// FIND FSM ///////////////////////////////////////
// -> FSM STATES ->
// 1. Idle
// 2. Patrol
// 3. Find
// 4. Attack
// 5. Grooming
// 6. SuccessedAttack
// 7. FailedAttack
// 8. Fall
// 9. Land
// 10. AfterAttack
// 11. Damage
// 12. Death

void CNormalEnemyScript::Idle()
{
    if (IsGround())
    {
        if (nullptr != GetTarget())
        {
            ChangeState(NormalEnemyState::Find);
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
        ChangeState(NormalEnemyState::Fall);
    }
}

void CNormalEnemyScript::Grooming()
{
    if (nullptr != GetTarget())
    {
        ChangeState(NormalEnemyState::Find);
    }
    else
    {
        if (CHECK_ANIMFRM(GetOwner(), 132))
        {
            GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
                TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png",
                                                            L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.03.png"));
        }

        if (CHECK_ANIMFRM(GetOwner(), 251))
        {
            GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
                TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png",
                                                            L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png"));
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
            ChangeState(NormalEnemyState::Find);
        }
    }
    else
    {
        ChangeState(NormalEnemyState::Fall);
    }
}

void CNormalEnemyScript::Find()
{
    RotatingToTarget();

    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 vTargetDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();

    if (vDir.Dot(vTargetDir.Normalize()) >= cosf(0.f) - 0.2f)
    {
        if (Animator()->IsFinish())
        {
            ChangeState(NormalEnemyState::Attack);
        }
    }
}

void CNormalEnemyScript::Attack()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;

    ApplyDir(vDir, true);

    m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);
    Rigidbody()->SetVelocity(vDir * m_fSpeed + Vec3(0.f, -9.8f, 0.f));

    m_fSpeed = Lerp(m_fSpeed, 0.f, m_fRushSpeedLerp * DT);

    if (m_fSpeed <= 3.f)
    {
        ChangeState(NormalEnemyState::Brake);
    }
}

void CNormalEnemyScript::Fall()
{
    if (IsGround())
    {
        ChangeState(NormalEnemyState::Land);
    }
}

void CNormalEnemyScript::Land()
{
    if (Animator()->IsFinish())
    {
        ChangeState(NormalEnemyState::AfterAttack);
    }
}

void CNormalEnemyScript::Brake()
{
    if (CHECK_ANIMFRM(GetOwner(), 30))
    {
        if (Animator()->IsFinish())
        {
            ChangeState(NormalEnemyState::AfterAttack);
        }
    }
}

void CNormalEnemyScript::Eaten()
{
    if (!GetResistState())
    {
        ChangeState(NormalEnemyState::Idle);
    }
}

void CNormalEnemyScript::AfterAttack()
{
    if (Animator()->IsFinish())
    {
        if (nullptr != GetTarget())
        {
            ChangeState(NormalEnemyState::Find);
            m_bEnter = false;
        }
        else
        {
            if (Animator()->IsFinish() && m_bEnter)
            {
                m_bCirclePatrol ? ChangeState(NormalEnemyState::Patrol) : ChangeState(RandomIdleState());
                m_bEnter = false;
            }
            else if (Animator()->IsFinish())
            {
                ChangeState(RandomIdleState());
            }
        }
    }
    else
    {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png",
                                                        L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemyEye.00.png"));
    }
}

void CNormalEnemyScript::Damage()
{
    if (GetCurInfo().HP <= 0.1f)
    {
        ChangeState(NormalEnemyState::Death);
    }
    else
    {
        if (Animator()->IsFinish())
        {
            ChangeState(NormalEnemyState::Fall);
            m_bFirst = false;
        }
    }
}

void CNormalEnemyScript::Death()
{
    if (Animator()->IsFinish())
    {
        SpawnDeadEffect(2);
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CNormalEnemyScript::Sleep()
{
    /* if (IsGetDamage())
     {
         ChangeState(RandomIdleState());
     }*/
}