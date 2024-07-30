#include "pch.h"
#include "CSirKibbleScript.h"

#include "CCutterProjectileScript.h"

CSirKibbleScript::CSirKibbleScript()
    : CMonsterUnitScript(SIRKIBBLESCRIPT)
    , m_eState(SIRKIBBLE_STATE::Idle)
    , m_pTargetObj(nullptr)
    , m_pAttackPoint(nullptr)
    , m_vDamageDir{}
    , m_bFlag(false)
    , m_fAccTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAccTime, "AccTime");
}

CSirKibbleScript::CSirKibbleScript(const CSirKibbleScript& _Origin)
    : CMonsterUnitScript(_Origin)
    , m_eState(SIRKIBBLE_STATE::Idle)
    , m_pTargetObj(nullptr)
    , m_pAttackPoint(nullptr)
    , m_vDamageDir{}
    , m_bFlag(false)
    , m_fAccTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAccTime, "AccTime");
}

CSirKibbleScript::~CSirKibbleScript()
{
}

void CSirKibbleScript::begin()
{
    m_pAttackPoint = GetOwner()->GetChildObject(L"Attack Point");

    if (nullptr == m_pAttackPoint)
    {
        MessageBox(nullptr, L"Attack Point Miss", L"SirKibbleScript", MB_OK);
        // TODO : 넣기
    }

    m_pAttackPoint->BoxCollider()->SetEnabled(false);
    ChangeState(SIRKIBBLE_STATE::Idle);
}

void CSirKibbleScript::tick()
{
    switch (m_eState)
    {
    case SIRKIBBLE_STATE::Idle: {
        Idle();
    }
    break;
        break;
    case SIRKIBBLE_STATE::Find: {
        Find();
    }
    break;
    case SIRKIBBLE_STATE::AirCutterJumpStart: {
        AirCutterJumpStart();
    }
    break;
    case SIRKIBBLE_STATE::AirCutterJump: {
        AirCutterJump();
    }
    break;
    case SIRKIBBLE_STATE::AirCutterThrow: {
        AirCutterThrow();
    }
    break;
    case SIRKIBBLE_STATE::CutterThrowStart: {
        CutterThrowStart();
    }
    break;
    case SIRKIBBLE_STATE::CutterThrowStartWait: {
        CutterThrowStartWait();
    }
    break;
    case SIRKIBBLE_STATE::CutterThrow: {
        CutterThrow();
    }
    break;
    case SIRKIBBLE_STATE::CutterCatch: {
        CutterCatch();
    }
    break;
    case SIRKIBBLE_STATE::Damage: {
        Damage();
    }
    break;
    case SIRKIBBLE_STATE::Eaten: {
        Eaten();
    }
    break;
    case SIRKIBBLE_STATE::Death: {
        Death();
    }
    break;
    case SIRKIBBLE_STATE::Fall: {
        Fall();
    }
    break;
    case SIRKIBBLE_STATE::Land: {
        Land();
    }
    break;
    case SIRKIBBLE_STATE::End:
        break;
    default:
        break;
    }
}

void CSirKibbleScript::EnterState(SIRKIBBLE_STATE _state)
{
    switch (m_eState)
    {
    case SIRKIBBLE_STATE::Idle: {
        Animator()->Play(ANIMPREFIX("Wait"));
    }
    break;
    case SIRKIBBLE_STATE::Find: {
        Animator()->Play(ANIMPREFIX("Find"), false);
    }
    break;
    case SIRKIBBLE_STATE::FindWait: {
        Animator()->Play(ANIMPREFIX("FindWait"));
    }
    break;
    case SIRKIBBLE_STATE::AirCutterJumpStart: {
        Animator()->Play(ANIMPREFIX("AirCutterJumpStart"), false);
    }
    break;
    case SIRKIBBLE_STATE::AirCutterJump: {
        Rigidbody()->AddForce(Vec3(0.f, 1.f, 0.f) * GetCurInfo().JumpPower, ForceMode::Impulse);
        Animator()->Play(ANIMPREFIX("AirCutterJump"), false);
    }
    break;
    case SIRKIBBLE_STATE::AirCutterThrow: {
        Rigidbody()->SetUseGravity(false);
        Animator()->Play(ANIMPREFIX("AirCutterThrow"), false);
        ProjectileAttack();
    }
    break;
    case SIRKIBBLE_STATE::CutterThrowStart: {
        Animator()->Play(ANIMPREFIX("CutterThrowStart"), false);
    }
    break;
    case SIRKIBBLE_STATE::CutterThrowStartWait: {
        Animator()->Play(ANIMPREFIX("CutterThrowStartWait"));
        m_pTargetObj = GetTarget();
    }
    break;
    case SIRKIBBLE_STATE::CutterThrow: {
        Animator()->Play(ANIMPREFIX("CutterThrow"), false);
        ProjectileAttack();
    }
    break;
    case SIRKIBBLE_STATE::CutterCatch: {
        m_pAttackPoint->BoxCollider()->SetEnabled(false);
        Animator()->Play(ANIMPREFIX("CutterCatch"), false);
    }
    break;
    case SIRKIBBLE_STATE::Damage: {
        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case SIRKIBBLE_STATE::Eaten: {
        Animator()->Play(ANIMPREFIX("Damage"));
    }
    break;
    case SIRKIBBLE_STATE::Fall: {
        Animator()->Play(ANIMPREFIX("Fall"));
    }
    break;
    case SIRKIBBLE_STATE::Land: {
        Animator()->Play(ANIMPREFIX("Landing"), false);
    }
    break;
    case SIRKIBBLE_STATE::Death:
        break;
    case SIRKIBBLE_STATE::End:
        break;
    default:
        break;
    }
}

void CSirKibbleScript::ChangeState(SIRKIBBLE_STATE _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CSirKibbleScript::ExitState(SIRKIBBLE_STATE _state)
{
    switch (m_eState)
    {
    case SIRKIBBLE_STATE::Idle: {
        m_fAccTime = 0.f;
    }
    break;
    case SIRKIBBLE_STATE::Find:
        break;
    case SIRKIBBLE_STATE::AirCutterJumpStart:
        break;
    case SIRKIBBLE_STATE::AirCutterJump:
        break;
    case SIRKIBBLE_STATE::AirCutterThrow: {
        Rigidbody()->SetUseGravity(true);
        m_pTargetObj = nullptr;
    }
    break;
    case SIRKIBBLE_STATE::CutterThrowStart:
        break;
    case SIRKIBBLE_STATE::CutterThrowStartWait:
        break;
    case SIRKIBBLE_STATE::CutterThrow: {
        m_pTargetObj = nullptr;
    }
    break;
    case SIRKIBBLE_STATE::CutterCatch:
        break;
    case SIRKIBBLE_STATE::Damage: {
        m_bFlag = false;
    }
    break;
    case SIRKIBBLE_STATE::Eaten: {
        m_bFlag = false;
    }
    break;
    case SIRKIBBLE_STATE::Death:
        break;
    case SIRKIBBLE_STATE::Land:
        break;
    case SIRKIBBLE_STATE::End:
        break;
    default:
        break;
    }
}

void CSirKibbleScript::Idle()
{
    m_fAccTime += DT;
    if (nullptr != GetTarget() && m_fAccTime >= 2.f)
    {
        ChangeState(SIRKIBBLE_STATE::Find);
    }
}

void CSirKibbleScript::Find()
{
    RotatingToTarget();
    if (Animator()->IsFinish())
    {
        if (nullptr != GetTarget())
        {
            if (GetTarget()->Transform()->GetLocalPos().y - Transform()->GetLocalPos().y >= 100.f)
            {
                ChangeState(SIRKIBBLE_STATE::AirCutterJumpStart);
            }
            else
            {
                ChangeState(SIRKIBBLE_STATE::CutterThrowStart);
            }
        }
        else
        {
            ChangeState(SIRKIBBLE_STATE::Idle);
        }
    }
}

void CSirKibbleScript::AirCutterJumpStart()
{
    Animator()->IsFinish() ? ChangeState(SIRKIBBLE_STATE::AirCutterJump) : void();
}

void CSirKibbleScript::AirCutterJump()
{
    Animator()->IsFinish() ? ChangeState(SIRKIBBLE_STATE::AirCutterThrow) : void();
}

void CSirKibbleScript::AirCutterThrow()
{
    Animator()->IsFinish() ? ChangeState(SIRKIBBLE_STATE::Fall) : void();
}

void CSirKibbleScript::CutterThrowStart()
{
    Animator()->IsFinish() ? ChangeState(SIRKIBBLE_STATE::CutterThrowStartWait) : void();
}

void CSirKibbleScript::CutterThrowStartWait()
{
    RotatingToTarget();
    if (nullptr != GetTarget())
    {
        Quat vDir = Transform()->GetWorldQuaternion();
        Vec3 vTargetDir = m_pTargetObj->Transform()->GetLocalPos() - Transform()->GetLocalPos();

        Vec3 vUP = vTargetDir == Vec3(0.f, 0.f, -1.f) ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);
        Quat vTargetQuat = Quat::LookRotation(-vTargetDir, vUP);

        if (vTargetQuat.Dot(vDir) < cosf(0.f) - 0.0000001f)
        {
            ChangeState(SIRKIBBLE_STATE::CutterThrow);
        }
    }
}

void CSirKibbleScript::CutterThrow()
{
    Animator()->IsFinish() ? ChangeState(SIRKIBBLE_STATE::FindWait) : void();
}

void CSirKibbleScript::CutterCatch()
{
    Animator()->IsFinish() ? ChangeState(SIRKIBBLE_STATE::Idle) : void();
}

void CSirKibbleScript::Damage()
{
    if (!m_bFlag)
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        m_vDamageDir.Normalize();
        m_vDamageDir.y = 1.5f;
        Rigidbody()->AddForce(m_vDamageDir * 50.f, ForceMode::Impulse);
        m_bFlag = true;
    }

    // 아마 Damage 받을 때 사망 띠~
    if (GetCurInfo().HP <= 0.f)
    {
        ChangeState(SIRKIBBLE_STATE::Death);
    }

    Animator()->IsFinish() ? ChangeState(SIRKIBBLE_STATE::Idle) : void();
}

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

void CSirKibbleScript::Land()
{
    Animator()->IsFinish() ? ChangeState(SIRKIBBLE_STATE::Idle) : void();
}

void CSirKibbleScript::Fall()
{
    // TODO : 나중에 들어오는 IsGround 함수 사용
    if (Rigidbody()->GetVelocity().y >= -0.01f)
    {
        ChangeState(SIRKIBBLE_STATE::Land);
    }
}

void CSirKibbleScript::Death()
{
    Animator()->IsFinish() ? GamePlayStatic::DestroyGameObject(GetOwner()) : void();
}

void CSirKibbleScript::ProjectileAttack()
{
    Ptr<CPrefab> bulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\SirKibbleBullet.pref", L"prefab\\SirKibbleBullet.pref");
    if (nullptr != bulletPref)
    {
        CGameObject* pBullet = bulletPref->Instantiate();
        pBullet->Transform()->SetLocalPos(m_pAttackPoint->Transform()->GetWorldPos());
        pBullet->Transform()->SetWorldRotation(Transform()->GetWorldQuaternion());

        pBullet->GetScript<CCutterProjectileScript>()->SetOwenr(GetOwner());
        pBullet->GetScript<CCutterProjectileScript>()->SetAttackPoint(m_pAttackPoint);
        // TODO : LAYER_MONSTER -> LAYER_MOSTERATK 변경
        GamePlayStatic::SpawnGameObject(pBullet, LAYER_MONSTER);
    }
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
    if (SIRKIBBLE_STATE::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();

    UnitHit hit;
    ZeroMemory(&hit, sizeof(hit));

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(SIRKIBBLE_STATE::Eaten);
            m_vDamageDir = -pObj->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            return;
        }

        UnitHit hitInfo = {};

        L"Body Collider" == pObj->GetName() ? pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo) : void();
    }

    // TODO : 돌아온 부메랑이 나에게 맞았다면 ChangeState(CutterCatch)
}

void CSirKibbleScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이는게 끝난 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(SIRKIBBLE_STATE::Idle);
        }
    }
}