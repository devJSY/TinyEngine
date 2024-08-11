#include "pch.h"
#include "CSirKibbleScript.h"

#include "CCutterProjectileScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CSirKibbleScript::CSirKibbleScript()
    : CMonsterUnitScript(SIRKIBBLESCRIPT)
    , m_eState(SirKibbleState::Idle)
    , m_pAttackPoint(nullptr)
    , m_vDamageDir{}
    , m_bFlag(false)
    , m_bJump(false)
    , m_fAccTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAccTime, "AccTime");
}

CSirKibbleScript::CSirKibbleScript(const CSirKibbleScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_eState(SirKibbleState::Idle)
    , m_pAttackPoint(nullptr)
    , m_vDamageDir{}
    , m_bFlag(false)
    , m_bJump(false)
    , m_fAccTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAccTime, "AccTime");
}

CSirKibbleScript::~CSirKibbleScript()
{
}

void CSirKibbleScript::begin()
{
    CUnitScript::begin();

    m_pAttackPoint = GetOwner()->GetChildObject(L"Attack Point");

    if (nullptr == m_pAttackPoint)
    {
        MessageBox(nullptr, L"Attack Point Miss", L"SirKibbleScript", MB_OK);
    }

    m_pAttackPoint->BoxCollider()->SetEnabled(false);
    ChangeState(SirKibbleState::Idle);
}

void CSirKibbleScript::tick()
{
    CUnitScript::tick();
    CheckDamage();
    FSM();
}

UINT CSirKibbleScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CSirKibbleScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);

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

void CSirKibbleScript::EnterState(SirKibbleState _state)
{
    switch (m_eState)
    {
    case SirKibbleState::Idle: {
        Animator()->Play(ANIMPREFIX("Wait"));
    }
    break;
    case SirKibbleState::Find: {
        Animator()->Play(ANIMPREFIX("Find"), false);
    }
    break;
    case SirKibbleState::FindWait: {
        Animator()->Play(ANIMPREFIX("FindWait"));
    }
    break;
    case SirKibbleState::AirCutterJumpStart: {
        Animator()->Play(ANIMPREFIX("AirCutterJumpStart"), false);
    }
    break;
    case SirKibbleState::AirCutterJump: {
        Rigidbody()->AddForce(Vec3(0.f, 1.f, 0.f) * GetCurInfo().JumpPower, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("AirCutterJump"), false);
    }
    break;
    case SirKibbleState::AirCutterThrow: {
        Rigidbody()->SetUseGravity(false);
        Animator()->Play(ANIMPREFIX("AirCutterThrow"), false);
        ProjectileAttack(true);
    }
    break;
    case SirKibbleState::CutterThrowStart: {
        Animator()->Play(ANIMPREFIX("CutterThrowStart"), false);
    }
    break;
    case SirKibbleState::CutterThrowStartWait: {
        Animator()->Play(ANIMPREFIX("CutterThrowStartWait"));
    }
    break;
    case SirKibbleState::CutterThrow: {
        Animator()->Play(ANIMPREFIX("CutterThrow"), false);
        ProjectileAttack(false);
    }
    break;
    case SirKibbleState::CutterCatch: {
        m_pAttackPoint->BoxCollider()->SetEnabled(false);
        Animator()->Play(ANIMPREFIX("CutterCatch"), false);
    }
    break;
    case SirKibbleState::Damage: {
        Transform()->SetDirection((PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize());

        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        Vec3 vHitDir = GetOwner()->GetScript<CUnitScript>()->GetHitDir();
        vHitDir.y = 1.5f;

        Rigidbody()->AddForce(vHitDir.Normalize() * 5.f, ForceMode::Impulse);

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
        Animator()->Play(ANIMPREFIX("Landing"), false);
    }
    break;
    case SirKibbleState::Death:
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
    }
    break;
    case SirKibbleState::CutterCatch:
        break;
    case SirKibbleState::Damage: {
    }
    break;
    case SirKibbleState::Eaten: {
        m_bFlag = false;
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

///////////////////////////// FIND FSM ///////////////////////////////////////
// -> FSM STATES ->
// 1. Idle
// 2. Find
// 3. AirCutterJumpStart
// 4. AirCutterJump
// 5. AirCutterThrow
// 6. CutterThrowStart
// 7. CutterThrowStartWait
// 8. CutterThrow
// 9. CutterCatch
// 10. Damage
// 11. Eaten
// 12. Fall
// 13. Land
// 14. Death

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
    Animator()->IsFinish() ? ChangeState(SirKibbleState::Idle) : void();
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
        if (vDot >= cosf(0.f) - 0.001f)
        {
            ChangeState(SirKibbleState::CutterThrow);
        }
    }
}
#pragma endregion

#pragma region CUTTERTHROW
void CSirKibbleScript::CutterThrow()
{
    Animator()->IsFinish() ? ChangeState(SirKibbleState::FindWait) : void();
}
#pragma endregion

#pragma region CUTTERCATCH
void CSirKibbleScript::CutterCatch()
{
    Animator()->IsFinish() ? ChangeState(SirKibbleState::Idle) : void();
}
#pragma endregion

#pragma region DAMAGE
void CSirKibbleScript::Damage()
{
    if (GetCurInfo().HP <= 0.f)
    {
        ChangeState(SirKibbleState::Death);
    }

    Animator()->IsFinish() ? ChangeState(SirKibbleState::Fall) : void();
}
#pragma endregion

#pragma region EATEN
void CSirKibbleScript::Eaten()
{
    // TODO : 저항 값 넣기
    if (!m_bFlag)
    {
        m_vDamageDir.Normalize();
        Vec3 vUp = Vec3(0.f, 0.f, -1.f) == m_vDamageDir ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);
        Quat vQuat = Quat::LookRotation(-m_vDamageDir, vUp);
        Transform()->SetWorldRotation(vQuat);
        m_bFlag = true;
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