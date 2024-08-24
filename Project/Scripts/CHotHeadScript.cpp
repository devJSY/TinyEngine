#include "pch.h"
#include "CHotHeadScript.h"

#include "CPlayerMgr.h"
#include "CFireProjectileScript.h"

CHotHeadScript::CHotHeadScript()
    : CMonsterUnitScript(HOTHEADSCRIPT)
    , m_pFlameRotObject(nullptr)
    , m_eState(HotHeadState::Idle)
    , m_vDamageDir{}
    , m_fAimingTime(0.f)
    , m_fAccTime(0.f)
    , m_fShotLength(0.f)
    , m_fFlameLength(0.f)
    , m_fFlameRotLength(0.f)
    , m_fRotRadian(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAimingTime, "Aiming Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAccTime, "AccTime");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fShotLength, "ShotLength");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fFlameLength, "FlameLength");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fFlameRotLength, "FlameRotLength");
}

CHotHeadScript::CHotHeadScript(const CHotHeadScript& _Origin)
    : CMonsterUnitScript(_Origin)
    , m_pFlameRotObject(nullptr)
    , m_eState(HotHeadState::Idle)
    , m_vDamageDir{}
    , m_fAimingTime(_Origin.m_fAccTime)
    , m_fAccTime(0.f)
    , m_fShotLength(_Origin.m_fShotLength)
    , m_fFlameLength(_Origin.m_fFlameLength)
    , m_fFlameRotLength(_Origin.m_fFlameRotLength)
    , m_fRotRadian(_Origin.m_fRotRadian)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAimingTime, "Aiming Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAccTime, "Aiming Time");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fShotLength, "ShotLength");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fFlameLength, "FlameLength");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fFlameRotLength, "FlameRotLength");
}

CHotHeadScript::~CHotHeadScript()
{
}

void CHotHeadScript::begin()
{
    CMonsterUnitScript::begin();

    GetOwner()->MeshRender()->GetDynamicMaterial(0);

    InitSetting();

    ChangeState(HotHeadState::Idle);
}

void CHotHeadScript::tick()
{
    CMonsterUnitScript::tick();

    CheckDamage();

    FSM();

    if (HotHeadState::Eaten != m_eState && GetResistState())
    {
        ChangeState(HotHeadState::Eaten);
    }
}

UINT CHotHeadScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);
    fwrite(&m_fAimingTime, sizeof(float), 1, _File);
    fwrite(&m_fShotLength, sizeof(float), 1, _File);
    fwrite(&m_fFlameLength, sizeof(float), 1, _File);
    fwrite(&m_fFlameRotLength, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CHotHeadScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);
    fread(&m_fAimingTime, sizeof(float), 1, _File);
    fread(&m_fShotLength, sizeof(float), 1, _File);
    fread(&m_fFlameLength, sizeof(float), 1, _File);
    fread(&m_fFlameRotLength, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

void CHotHeadScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (HotHeadState::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();
    bool flag = false;

    UnitHit hit;
    ZeroMemory(&hit, sizeof(hit));
    /**********************
    | 2. Player Body Hit
    ***********************/
    // 충돌한 오브젝트가 PlayerBody인지 확인

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(HotHeadState::Eaten);
            m_vDamageDir = -pObj->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
            return;
        }
    }

    Vec3 vDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();
    UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, vDir.Normalize(), GetCurInfo().ATK, 0.f, 0.f};
    UINT Layer = _OtherCollider->GetOwner()->GetLayerIdx();

    if (Layer == LAYER_PLAYER_TRIGGER && L"Body Collider" == pObj->GetName())
    {
        pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo);
    }
}

void CHotHeadScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이는게 끝난 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(HotHeadState::Idle);
        }
    }
}

/////////////////////////// FUNC //////////////////////////////////
// 1. InitSetting
// 2. EnterState
// 3. FSM
// 4. ExitState
// 5. ChangeState
// 6. CheckDamage
// 7. Projectile Attack

void CHotHeadScript::InitSetting()
{
    m_pFlameRotObject = GetOwner()->GetChildObject(L"FlameRot Object");

    if (nullptr == m_pFlameRotObject)
    {
        MessageBox(nullptr, L"FlameRot Object Does Not Exist", L"FlameRot Object Issue", MB_OK);
        return;
        // TODO : 위치 정해지면 코드로 생성해주기
    }

    m_pFlameRotObject->SetActive(false);

    GetOwner()->MeshRender()->GetDynamicMaterial(0);
}

void CHotHeadScript::EnterState(HotHeadState _state)
{
    switch (m_eState)
    {
    case HotHeadState::Idle: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.00.png",
                                                        L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.00.png"));
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("Wait"), true, false, 1.5f);
    }
    break;
    case HotHeadState::Find: {
        Animator()->Play(ANIMPREFIX("Find"), false, false, 1.5f);
    }
    break;
    case HotHeadState::Aiming: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.05.png",
                                                        L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.05.png"));
        Animator()->Play(ANIMPREFIX("Walk"), true, false, 1.5f);
    }
    break;
    case HotHeadState::AttackShootStart: {
        Animator()->Play(ANIMPREFIX("AttackShootStart"), false, false, 1.5f);
    }
    break;
    case HotHeadState::AttackShoot: {
        ProjectileAttack();
        Animator()->Play(ANIMPREFIX("AttackShoot"), false, false, 1.5f);
    }
    break;
    case HotHeadState::AttackShootEnd: {
        Animator()->Play(ANIMPREFIX("AttackShootEnd"), false, false, 1.5f);
    }
    break;
    case HotHeadState::AttackFlameRotStart: {
        Animator()->Play(ANIMPREFIX("AttackFlameRotStart"), false, false, 1.5f);
    }
    break;
    case HotHeadState::AttackFlameRot: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.00.png",
                                                        L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.00.png"));
        // 현재 Radian 가져오기
        m_fRotRadian = m_pFlameRotObject->Transform()->GetLocalRotation().y;
        // TODO : 앞에 Attack Area 키기
        m_pFlameRotObject->SetActive(true);
        Animator()->Play(ANIMPREFIX("AttackFlameRot"), false, false, 1.5f);
    }
    break;
    case HotHeadState::AttackFlameRotEnd: {
        // TODO : 앞에 Attack Area 끄기
        m_pFlameRotObject->SetActive(false);
        Animator()->Play(ANIMPREFIX("AttackFlameRotEnd"), false);
    }
    break;
    case HotHeadState::Fall: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.03.png",
                                                        L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.03.png"));
        Animator()->Play(ANIMPREFIX("Fall"));
    }
    break;
    case HotHeadState::Landing: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.00.png",
                                                        L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.00.png"));
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("Landing"), false);
    }
    break;
    case HotHeadState::Damage: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.03.png",
                                                        L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.03.png"));

        SetSparkle(true);

        Transform()->SetDirection((PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize());

        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        Vec3 vHitDir = GetOwner()->GetScript<CUnitScript>()->GetHitDir();
        float fForce = 0.f;
        float fDamageTime = 0.f;
        if (GetCurInfo().HP <= 0.1f)
        {
            fForce = 14.f;
            vHitDir.y = 1.5f;
            fDamageTime = 0.6f;
        }
        else
        {
            fForce = 14.f;
            vHitDir.y = 1.2f;
            fDamageTime = 2.f;
        }

        Rigidbody()->AddForce(vHitDir.Normalize() * fForce, ForceMode::Impulse);

        Animator()->Play(ANIMPREFIX("Damage"), false, false, fDamageTime);
    }
    break;
    case HotHeadState::Eaten: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.03.png",
                                                        L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.03.png"));
        Animator()->Play(ANIMPREFIX("Damage"));
    }
    break;
    case HotHeadState::Death: {
        GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(
            TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.03.png",
                                                        L"fbx\\Characters\\Monster\\HotHead\\FaceTexturePattern.03.png"));
        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case HotHeadState::End:
        break;
    default:
        break;
    }
}

void CHotHeadScript::FSM()
{
    switch (m_eState)
    {
    case HotHeadState::Idle: {
        Idle();
    }
    break;
    case HotHeadState::Find: {
        Find();
    }
    break;
    case HotHeadState::Aiming: {
        Aiming();
    }
    break;
    case HotHeadState::AttackShootStart: {
        AttackShootStart();
    }
    break;
    case HotHeadState::AttackShoot: {
        AttackShoot();
    }
    break;
    case HotHeadState::AttackFlameStart: {
        AttackFlameStart();
    }
    break;
    case HotHeadState::AttackFlame: {
        AttackFlame();
    }
    break;
    case HotHeadState::AttackFlameRotStart: {
        AttackFlameRotStart();
    }
    break;
    case HotHeadState::AttackFlameRot: {
        AttackFlameRot();
    }
    break;
    case HotHeadState::AttackShootEnd:
    case HotHeadState::AttackFlameEnd:
    case HotHeadState::AttackFlameRotEnd: {
        AttackEnd();
    }
    break;
    case HotHeadState::Fall: {
        Fall();
    }
    break;
    case HotHeadState::Landing: {
        Landing();
    }
    break;
    case HotHeadState::Damage: {
        m_pFlameRotObject->SetActive(false);
        Damage();
    }
    break;
    case HotHeadState::Eaten: {
        Eaten();
    }
    break;
    case HotHeadState::Death: {
        Death();
    }
    break;
    case HotHeadState::End:
        break;
    default:
        break;
    }
}

void CHotHeadScript::ExitState(HotHeadState _state)
{
    switch (m_eState)
    {
    case HotHeadState::Idle: {
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, false);
        m_fAccTime = 0.f;
    }
    break;
    case HotHeadState::Find:
        break;
    case HotHeadState::Aiming: {
        m_fAccTime = 0.f;
    }
    break;
    case HotHeadState::AttackShootStart:
        break;
    case HotHeadState::AttackShoot:
        break;
    case HotHeadState::AttackShootEnd:
        break;
    case HotHeadState::AttackFlameStart:
        break;
    case HotHeadState::AttackFlame: {
        m_fAccTime = 0.f;
    }
    break;
    case HotHeadState::AttackFlameEnd:
        break;
    case HotHeadState::AttackFlameRotStart: {
    }
    break;
    case HotHeadState::AttackFlameRot: {
        // 원래 Rotation으로 초기화
        m_fRotRadian = 0.f;
        m_pFlameRotObject->Transform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case HotHeadState::AttackFlameRotEnd:
        break;
    case HotHeadState::Damage:
    case HotHeadState::Eaten: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case HotHeadState::Death:
        break;
    case HotHeadState::End:
        break;
    default:
        break;
    }
}

void CHotHeadScript::ChangeState(HotHeadState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CHotHeadScript::CheckDamage()
{
    if (IsGetDamage())
    {
        ChangeState(HotHeadState::Damage);
    }
}

void CHotHeadScript::ProjectileAttack()
{
    Ptr<CPrefab> bulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\HotHeadBullet.pref", L"prefab\\HotHeadBullet.pref");
    if (nullptr != bulletPref)
    {
        CGameObject* pBullet = bulletPref->Instantiate();
        pBullet->Transform()->SetLocalPos(Transform()->GetWorldPos() + Transform()->GetWorldDir(DIR_TYPE::FRONT) * 10.f + Vec3(0.f, 40.f, 0.f));
        pBullet->Transform()->SetWorldRotation(Transform()->GetWorldQuaternion());
        GamePlayStatic::SpawnGameObject(pBullet, pBullet->GetLayerIdx());
    }
}

///////////////////////////// FIND FSM ///////////////////////////////////////
// -> FSM STATES ->
// 1. Idle
// 2. Find
// 3. Aiming
// 4. AttackShootStart
// 5. AttackShoot
// 6. AttackFlameStart
// 7. AttackFlame
// 8. AttackFlameRotStart
// 9. AttackFlameRot
// 10. Damage
// 11. Eaten
// 12. Death

#pragma region IDLE
void CHotHeadScript::Idle()
{
    m_fAccTime += DT;
    if (nullptr != GetTarget() && m_fAccTime >= 2.f)
    {
        ChangeState(HotHeadState::Find);
    }
}
#pragma endregion

#pragma region FIND
void CHotHeadScript::Find()
{
    if (Animator()->IsFinish())
    {
        ChangeState(HotHeadState::Aiming);
    }
}
#pragma endregion

#pragma region AIMING
void CHotHeadScript::Aiming()
{
    RotatingToTarget();

    Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 vTargetDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();

    float fDot = vFront.Dot(vTargetDir.Normalize());

    if (fDot >= 0.999f)
    {
        float fDistance = (PLAYER->GetComponent<CTransform>()->GetLocalPos() - Transform()->GetLocalPos()).Length();
        HotHeadState state = {};

        if (fDistance <= m_fShotLength)
        {
            state = HotHeadState::AttackFlameRotStart;
        }
        else
        {
            state = HotHeadState::AttackShootStart;
        }

        ChangeState(state);
    }
}
#pragma endregion

#pragma region ATTACKSHOOTSTART
void CHotHeadScript::AttackShootStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(HotHeadState::AttackShoot);
    }
}
#pragma endregion

#pragma region ATTACKSHOT
void CHotHeadScript::AttackShoot()
{
    if (Animator()->IsFinish())
    {
        ChangeState(HotHeadState::AttackShootEnd);
    }
}
void CHotHeadScript::AttackFlameStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(HotHeadState::AttackFlame);
    }
}
void CHotHeadScript::AttackFlame()
{
    m_fAccTime += DT;
    if (Animator()->IsFinish() && m_fAccTime >= 3.f)
    {
        ChangeState(HotHeadState::AttackFlameEnd);
    }
}
#pragma endregion

#pragma region ATTACKFLAMEROTSTART
void CHotHeadScript::AttackFlameRotStart()
{
    if (Animator()->IsFinish())
    {
        ChangeState(HotHeadState::AttackFlameRot);
    }
}
#pragma endregion

#pragma region ATTACKFLAMEROT
void CHotHeadScript::AttackFlameRot()
{
    Vec3 vRadian = m_pFlameRotObject->Transform()->GetLocalRotation();

    vRadian.y += GetCurInfo().RotationSpeed * DT;

    m_pFlameRotObject->Transform()->SetLocalRotation(vRadian);

    if (Animator()->IsFinish())
    {
        ChangeState(HotHeadState::AttackFlameRotEnd);
    }
}
#pragma endregion

#pragma region ATTACKEND
void CHotHeadScript::AttackEnd()
{
    Animator()->IsFinish() ? ChangeState(HotHeadState::Idle) : void();
}
#pragma endregion

#pragma region FALL
void CHotHeadScript::Fall()
{
    if (IsGround())
    {
        ChangeState(HotHeadState::Landing);
    }
}
#pragma endregion

#pragma region LANDING
void CHotHeadScript::Landing()
{
    if (Animator()->IsFinish())
    {
        ChangeState(HotHeadState::Idle);
    }
}
#pragma endregion

#pragma region DAMAGE
void CHotHeadScript::Damage()
{
    if (GetCurInfo().HP <= 0.f)
    {
        ChangeState(HotHeadState::Death);
    }

    Animator()->IsFinish() ? ChangeState(HotHeadState::Fall) : void();
}
#pragma endregion

#pragma region EATEN
void CHotHeadScript::Eaten()
{
    if (!GetResistState())
    {
        ChangeState(HotHeadState::Idle);
    }
}
#pragma endregion

#pragma region DEATH
void CHotHeadScript::Death()
{
    Animator()->IsFinish() ? GamePlayStatic::DestroyGameObject(GetOwner()) : void();
}
#pragma endregion