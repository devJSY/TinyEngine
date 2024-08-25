#include "pch.h"
#include "CSirKibbleScript.h"

#include "CCutterProjectileScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CSirKibbleScript::CSirKibbleScript()
    : CMonsterUnitScript(SIRKIBBLESCRIPT)
    , m_eState(SirKibbleState::Idle)
    , m_pAttackPoint(nullptr)
    , m_vOriginPos{}
    , m_vDamageDir{}
    , m_vDestPos{}
    , m_bJump(false)
    , m_fAccTime(0.f)
    , m_bPatrol(false)
    , m_bThrow(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDestPos, "Destination");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAccTime, "AccTime");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bPatrol, "Patrol");
}

CSirKibbleScript::CSirKibbleScript(const CSirKibbleScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_eState(SirKibbleState::Idle)
    , m_pAttackPoint(nullptr)
    , m_vDamageDir{}
    , m_vOriginPos{}
    , m_vDestPos{}
    , m_bJump(false)
    , m_fAccTime(0.f)
    , m_bPatrol(false)
    , m_bThrow(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDestPos, "Destination");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAccTime, "AccTime");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bPatrol, "Patrol");
}

CSirKibbleScript::~CSirKibbleScript()
{
}

void CSirKibbleScript::begin()
{
    CMonsterUnitScript::begin();

    m_pAttackPoint = GetOwner()->GetChildObject(L"Attack Point");

    if (nullptr == m_pAttackPoint)
    {
        MessageBox(nullptr, L"Attack Point Miss", L"SirKibbleScript", MB_OK);
    }

    m_pAttackPoint->BoxCollider()->SetEnabled(false);

    if (m_bPatrol)
    {
        m_vOriginPos = Transform()->GetWorldPos();
        ChangeState(SirKibbleState::Patrol);
    }
    else
    {
        ChangeState(SirKibbleState::Idle);
    }
}

void CSirKibbleScript::tick()
{
    CMonsterUnitScript::tick();

    CheckDamage();

    FSM();

    if (SirKibbleState::Eaten != m_eState && GetResistState())
    {
        ChangeState(SirKibbleState::Eaten);
    }
}

UINT CSirKibbleScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);
    fwrite(&m_bPatrol, sizeof(bool), 1, _File);
    fwrite(&m_vDestPos, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}

UINT CSirKibbleScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);
    fread(&m_bPatrol, sizeof(bool), 1, _File);
    fread(&m_vDestPos, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec3);

    return MemoryByte;
}

void CSirKibbleScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (SirKibbleState::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(SirKibbleState::Eaten);
            m_vDamageDir = -pObj->Transform()->GetWorldDir(DIR_TYPE::FRONT);
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

void CSirKibbleScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이는게 끝난 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(SirKibbleState::Idle);
        }
    }
}

/////////////////////////// FUNC //////////////////////////////////
// 1. EnterState
// 2. FSM
// 3. ExitState
// 4. CheckDamage
// 5. Projectile Attack
// 6. ChangeState
// 7. LinearMove

void CSirKibbleScript::EnterState(SirKibbleState _state)
{
    switch (m_eState)
    {
    case SirKibbleState::Idle: {
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        Animator()->Play(ANIMPREFIX("Wait"), true, false, 1.5f);
    }
    break;
    case SirKibbleState::Patrol: {
        Animator()->Play(ANIMPREFIX("Walk"), true, false, 1.5f);
    }
    break;
    case SirKibbleState::PatrolRotating: {
        Animator()->Play(ANIMPREFIX("Walk"), true, false, 1.5f);
    }
    break;
    case SirKibbleState::Find: {
        Animator()->Play(ANIMPREFIX("Find"), false, false, 1.5f);
    }
    break;
    case SirKibbleState::FindWait: {
        Animator()->Play(ANIMPREFIX("FindWait"), true, false, 1.5f);
    }
    break;
    case SirKibbleState::AirCutterJumpStart: {
        Animator()->Play(ANIMPREFIX("AirCutterJumpStart"), false, false, 1.5f);
    }
    break;
    case SirKibbleState::AirCutterJump: {
        Rigidbody()->AddForce(Vec3(0.f, 1.f, 0.f) * GetCurInfo().JumpPower, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("AirCutterJump"), false, false, 1.5f);
    }
    break;
    case SirKibbleState::AirCutterThrow: {
        Rigidbody()->SetUseGravity(false);
        m_bThrow = true;
        Animator()->Play(ANIMPREFIX("AirCutterThrow"), false, false, 1.5f);
    }
    break;
    case SirKibbleState::CutterThrowStart: {
        Animator()->Play(ANIMPREFIX("CutterThrowStart"), false, false, 1.5f);
    }
    break;
    case SirKibbleState::CutterThrowStartWait: {
        Animator()->Play(ANIMPREFIX("CutterThrowStartWait"), true, false, 1.5f);
    }
    break;
    case SirKibbleState::CutterThrow: {
        m_bThrow = true;
        Animator()->Play(ANIMPREFIX("CutterThrow"), false, false, 1.5f);
    }
    break;
    case SirKibbleState::CutterCatch: {
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        m_pAttackPoint->BoxCollider()->SetEnabled(false);
        Animator()->Play(ANIMPREFIX("CutterCatch"), false, false, 1.5f);
    }
    break;
    case SirKibbleState::Damage: {
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
            fForce = 12.f;
            vHitDir.y = 1.5f;
        }
        else
        {
            fForce = 9.f;
            vHitDir.y = 0.8f;
        }

        Rigidbody()->AddForce(vHitDir.Normalize() * fForce, ForceMode::Impulse);

        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case SirKibbleState::Eaten: {
        Animator()->Play(ANIMPREFIX("Damage"));
    }
    break;
    case SirKibbleState::Fall: {
        Animator()->Play(ANIMPREFIX("Fall"));
    }
    break;
    case SirKibbleState::Land: {
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);

        Animator()->Play(ANIMPREFIX("Landing"), false);
    }
    break;
    case SirKibbleState::Death: {
        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case SirKibbleState::End:
        break;
    default:
        break;
    }
}

void CSirKibbleScript::FSM()
{
    switch (m_eState)
    {
    case SirKibbleState::Idle: {
        Idle();
    }
    break;
        break;
    case SirKibbleState::Patrol: {
        Patrol();
    }
    break;
    case SirKibbleState::PatrolRotating: {
        PatrolRotating();
    }
    break;
        break;
    case SirKibbleState::Find: {
        Find();
    }
    break;
    case SirKibbleState::FindWait: {
        FindWait();
    }
    break;
    case SirKibbleState::AirCutterJumpStart: {
        AirCutterJumpStart();
    }
    break;
    case SirKibbleState::AirCutterJump: {
        AirCutterJump();
    }
    break;
    case SirKibbleState::AirCutterThrow: {
        AirCutterThrow();
    }
    break;
    case SirKibbleState::CutterThrowStart: {
        CutterThrowStart();
    }
    break;
    case SirKibbleState::CutterThrowStartWait: {
        CutterThrowStartWait();
    }
    break;
    case SirKibbleState::CutterThrow: {
        CutterThrow();
    }
    break;
    case SirKibbleState::CutterCatch: {
        CutterCatch();
    }
    break;
    case SirKibbleState::Damage: {
        Damage();
    }
    break;
    case SirKibbleState::Eaten: {
        Eaten();
    }
    break;
    case SirKibbleState::Death: {
        Death();
    }
    break;
    case SirKibbleState::Fall: {
        Fall();
    }
    break;
    case SirKibbleState::Land: {
        Land();
    }
    break;
    case SirKibbleState::End:
        break;
    default:
        break;
    }
}

void CSirKibbleScript::ExitState(SirKibbleState _state)
{
    switch (m_eState)
    {
    case SirKibbleState::Idle: {
        Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, false);
        m_fAccTime = 0.f;
    }
    break;
    case SirKibbleState::Find:
        break;
    case SirKibbleState::AirCutterJumpStart:
        break;
    case SirKibbleState::AirCutterJump:
        break;
    case SirKibbleState::AirCutterThrow: {
        m_bThrow = false;
        Rigidbody()->SetUseGravity(true);
    }
    break;
    case SirKibbleState::CutterThrowStart:
        break;
    case SirKibbleState::CutterThrowStartWait: {
        m_bJump = false;
    }
    break;
    case SirKibbleState::CutterThrow: {
        m_bThrow = false;
    }
    break;
    case SirKibbleState::CutterCatch:
        break;
    case SirKibbleState::Damage: {
    }
    break;
    case SirKibbleState::Eaten: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case SirKibbleState::Death:
        break;
    case SirKibbleState::Land:
        break;
    case SirKibbleState::End:
        break;
    default:
        break;
    }
}

void CSirKibbleScript::CheckDamage()
{
    if (IsGetDamage())
    {
        ChangeState(SirKibbleState::Damage);
    }
}

void CSirKibbleScript::ProjectileAttack(bool _bFlag)
{
    Ptr<CPrefab> bulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\SirKibbleBullet.pref", L"prefab\\SirKibbleBullet.pref");
    if (nullptr != bulletPref)
    {
        CGameObject* pBullet = bulletPref->Instantiate();
        pBullet->Transform()->SetLocalPos(m_pAttackPoint->Transform()->GetWorldPos());
        pBullet->Transform()->SetWorldRotation(Transform()->GetWorldQuaternion());

        CCutterProjectileScript* pScript = pBullet->GetScript<CCutterProjectileScript>();

        pScript->SetOwenr(GetOwner());
        pScript->SetAttackPoint(m_pAttackPoint);

        if (_bFlag)
            pScript->SetJumpBack(_bFlag);

        // TODO : LAYER_MONSTER -> LAYER_MOSTERATK 변경
        GamePlayStatic::SpawnGameObject(pBullet, pBullet->GetLayerIdx());
    }
}

void CSirKibbleScript::ChangeState(SirKibbleState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CSirKibbleScript::LinearMove()
{
    Vec3 vPos = Transform()->GetWorldPos();
    Vec3 vUP = Transform()->GetWorldDir(DIR_TYPE::UP);

    Vec3 vDir = m_vDestPos - vPos;
    vDir.Normalize();
    vDir.y = 0;
    Transform()->SetDirection(vDir);

    Rigidbody()->SetVelocity(Transform()->GetWorldDir(DIR_TYPE::FRONT) * GetCurInfo().Speed * DT);

    if ((m_vDestPos.x - 5.f <= vPos.x && vPos.x <= m_vDestPos.x + 5.f) && (m_vDestPos.z - 5.f <= vPos.z && vPos.z <= m_vDestPos.z + 5.f))
    {
        Vec3 vTemp = m_vDestPos;
        m_vDestPos = m_vOriginPos;
        m_vOriginPos = vTemp;
        ChangeState(SirKibbleState::PatrolRotating);
    }
}

///////////////////////////// FIND FSM ///////////////////////////////////////
// -> FSM STATES ->
// 1. Idle
// 2. Patrol
// 3. Find
// 4. AirCutterJumpStart
// 5. AirCutterJump
// 6. AirCutterThrow
// 7. CutterThrowStart
// 8. CutterThrowStartWait
// 9. CutterThrow
// 10. CutterCatch
// 11. Damage
// 12. Eaten
// 13. Fall
// 14. Land
// 15. Death

#pragma region IDLE
void CSirKibbleScript::Idle()
{
    m_fAccTime += DT;
    if (nullptr != GetTarget() && PLAYER->GetScript<CUnitScript>()->GetCurInfo().HP > 0.01f && m_fAccTime >= 2.f)
    {
        ChangeState(SirKibbleState::Find);
    }
}

#pragma endregion

#pragma region PATROL
void CSirKibbleScript::Patrol()
{
    LinearMove();

    if (nullptr != GetTarget())
    {
        ChangeState(SirKibbleState::Find);
    }
}
#pragma endregion

#pragma region PATROLROTATING
void CSirKibbleScript::PatrolRotating()
{
    Vec3 ToTargetDir = m_vDestPos - Transform()->GetWorldPos();
    ToTargetDir.y = 0.f; // Y축 고정
    ToTargetDir.Normalize();
    Transform()->Slerp(ToTargetDir, DT * m_CurInfo.RotationSpeed);

    float fRadian = Transform()->GetWorldDir(DIR_TYPE::FRONT).Dot(ToTargetDir);
    if (fRadian >= cosf(0) - 0.01f)
    {
        ChangeState(SirKibbleState::Patrol);
    }
}
#pragma endregion

#pragma region FIND
void CSirKibbleScript::Find()
{
    Vec3 ToTargetDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();
    ToTargetDir.y = 0.f;
    Transform()->Slerp(ToTargetDir, DT * m_CurInfo.RotationSpeed);

    if (Animator()->IsFinish())
    {
        float fDiff = PLAYER->Transform()->GetWorldPos().y - Transform()->GetWorldPos().y;
        if (fDiff >= 70.f)
        {
            m_bJump = true;
        }
        ChangeState(SirKibbleState::CutterThrowStartWait);
    }
}
#pragma endregion

#pragma region FINDWAIT
void CSirKibbleScript::FindWait()
{
    if (Animator()->IsFinish())
    {
        m_bPatrol ? ChangeState(SirKibbleState::Patrol) : ChangeState(SirKibbleState::Idle);
    }
}
#pragma endregion

#pragma region AIRCUTTERJUMPSTART
void CSirKibbleScript::AirCutterJumpStart()
{
    Animator()->IsFinish() ? ChangeState(SirKibbleState::AirCutterJump) : void();
}
#pragma endregion

#pragma region AIRCUTTERJUMP
void CSirKibbleScript::AirCutterJump()
{
    Animator()->IsFinish() ? ChangeState(SirKibbleState::AirCutterThrow) : void();
}
#pragma endregion

#pragma region AIRCUTTERTHROW
void CSirKibbleScript::AirCutterThrow()
{
    if (CHECK_ANIMFRM(GetOwner(), 13) && m_bThrow)
    {
        ProjectileAttack(false);
        m_bThrow = false;
    }

    Animator()->IsFinish() ? ChangeState(SirKibbleState::Fall) : void();
}
#pragma endregion

#pragma region CUTTERTHROWSTART
void CSirKibbleScript::CutterThrowStart()
{
    Animator()->IsFinish() ? ChangeState(SirKibbleState::CutterThrowStartWait) : void();
}
#pragma endregion

#pragma region CUTTERTHROWSTARTWAIT
void CSirKibbleScript::CutterThrowStartWait()
{
    Vec3 ToTargetDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();
    ToTargetDir.y = 0.f;
    Transform()->Slerp(ToTargetDir, DT * m_CurInfo.RotationSpeed);

    Vec3 vPlayerPos = PLAYER->Transform()->GetWorldPos();
    Vec3 vPos = Transform()->GetWorldPos();

    vPlayerPos.y = 0.f;
    vPos.y = 0.f;

    Vec3 vTargetDir = vPlayerPos - vPos;
    Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

    vTargetDir.Normalize();
    vFront.y = 0.f;

    float vDot = vTargetDir.Dot(vFront);

    if (m_bJump)
    {
        if (vDot >= 0.8f)
        {
            ChangeState(SirKibbleState::AirCutterJumpStart);
        }
    }
    else
    {
        if (vDot >= cosf(0.f) - 0.02f)
        {
            ChangeState(SirKibbleState::CutterThrow);
        }
    }
}
#pragma endregion

#pragma region CUTTERTHROW
void CSirKibbleScript::CutterThrow()
{
    if (CHECK_ANIMFRM(GetOwner(), 13) && m_bThrow)
    {
        ProjectileAttack(false);
        m_bThrow = false;
    }

    Animator()->IsFinish() ? ChangeState(SirKibbleState::FindWait) : void();
}
#pragma endregion

#pragma region CUTTERCATCH
void CSirKibbleScript::CutterCatch()
{
    if (Animator()->IsFinish())
    {
        m_bPatrol ? ChangeState(SirKibbleState::Patrol) : ChangeState(SirKibbleState::Idle);
    }
}
#pragma endregion

#pragma region DAMAGE
void CSirKibbleScript::Damage()
{
    if (GetCurInfo().HP <= 0.1f)
    {
        ChangeState(SirKibbleState::Death);
    }

    Animator()->IsFinish() ? ChangeState(SirKibbleState::Fall) : void();
}
#pragma endregion

#pragma region EATEN
void CSirKibbleScript::Eaten()
{
    if (!GetResistState())
    {
        ChangeState(SirKibbleState::Idle);
    }
}
#pragma endregion

#pragma region LAND
void CSirKibbleScript::Land()
{
    Animator()->IsFinish() ? ChangeState(SirKibbleState::FindWait) : void();
}
#pragma endregion

#pragma region FALL
void CSirKibbleScript::Fall()
{
    if (IsGround())
    {
        ChangeState(SirKibbleState::Land);
    }
}
#pragma endregion

#pragma region DEATH
void CSirKibbleScript::Death()
{
    Animator()->IsFinish() ? GamePlayStatic::DestroyGameObject(GetOwner()) : void();
}
#pragma endregion