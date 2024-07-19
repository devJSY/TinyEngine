#include "pch.h"
#include "CCutterProjectileScript.h"

#include "CUnitScript.h"
#include "CSirKibbleScript.h"

CCutterProjectileScript::CCutterProjectileScript()
    : CMonsterATKScript(CUTTERPROJECTILESCRIPT)
    , m_pOwner(nullptr)
    , m_pAttackPoint(nullptr)
    , m_eState(CUTTERPROJECTILE_STATE::Attack)
    , m_fSpeed(0.f)
    , m_fAccTime(0.f)
    , m_vOriginPos{}
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Bullet Speed");
}

CCutterProjectileScript::CCutterProjectileScript(const CCutterProjectileScript& _Origin)
    : CMonsterATKScript(_Origin)
    , m_pOwner(nullptr)
    , m_pAttackPoint(nullptr)
    , m_eState(CUTTERPROJECTILE_STATE::Attack)
    , m_fSpeed(_Origin.m_fSpeed)
    , m_fAccTime(0.f)
    , m_vOriginPos{}
{
}

CCutterProjectileScript::~CCutterProjectileScript()
{
}

void CCutterProjectileScript::begin()
{
    ChangeState(CUTTERPROJECTILE_STATE::Attack);
}

void CCutterProjectileScript::tick()
{
    switch (m_eState)
    {
    case CUTTERPROJECTILE_STATE::Attack: {
        Attack();
    }
    break;
    case CUTTERPROJECTILE_STATE::AttackStop: {
        AttackStop();
    }
    break;
    case CUTTERPROJECTILE_STATE::AttackBack: {
        AttackBack();
    }
    break;
    case CUTTERPROJECTILE_STATE::Destroy: {
        Destroy();
    }
    break;
    case CUTTERPROJECTILE_STATE::End:
        break;
    default:
        break;
    }
}

void CCutterProjectileScript::EnterState(CUTTERPROJECTILE_STATE _state)
{
    switch (m_eState)
    {
    case CUTTERPROJECTILE_STATE::Attack: {
        Animator()->Play(ANIMPREFIX(L"SpinBig"));
        m_vOriginPos = Transform()->GetWorldPos();
    }
    break;
    case CUTTERPROJECTILE_STATE::AttackStop: {
        m_pAttackPoint->BoxCollider()->SetEnabled(true);
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case CUTTERPROJECTILE_STATE::AttackBack: {
        Vec3 vDir = m_pAttackPoint->Transform()->GetWorldPos() - Transform()->GetLocalPos();
        Vec3 vUP = vDir == Vec3(0.f, 0.f, -1.f) ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);

        Quat quat = Quat::LookRotation(-vDir, vUP);

        Transform()->SetWorldRotation(quat);
    }
    break;
    case CUTTERPROJECTILE_STATE::End:
        break;
    default:
        break;
    }
}

void CCutterProjectileScript::ChangeState(CUTTERPROJECTILE_STATE _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CCutterProjectileScript::ExitState(CUTTERPROJECTILE_STATE _state)
{
    switch (m_eState)
    {
    case CUTTERPROJECTILE_STATE::Attack: {
    }
    break;
    case CUTTERPROJECTILE_STATE::AttackStop: {
        m_fAccTime = 0.f;
    }
    break;
    case CUTTERPROJECTILE_STATE::AttackBack: {
    }
    break;
    case CUTTERPROJECTILE_STATE::End:
        break;
    default:
        break;
    }
}

void CCutterProjectileScript::Attack()
{
    Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    vDir.y = 0.f;
    Rigidbody()->SetVelocity(vDir * m_fSpeed);

    if ((Transform()->GetLocalPos() - m_vOriginPos).Length() >= 1000.f)
    {
        ChangeState(CUTTERPROJECTILE_STATE::AttackStop);
    }
}

void CCutterProjectileScript::AttackStop()
{
    m_fAccTime += DT;

    if (m_fAccTime >= 1.5f)
    {
        ChangeState(CUTTERPROJECTILE_STATE::AttackBack);
    }
}

void CCutterProjectileScript::AttackBack()
{
    Rigidbody()->SetVelocity(Transform()->GetWorldDir(DIR_TYPE::FRONT) * (m_fSpeed * 1.5f));
}

void CCutterProjectileScript::Destroy()
{
    // TODO : 삭제 되기전에 할거 하기
    GamePlayStatic::DestroyGameObject(GetOwner());
}

void CCutterProjectileScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    // 1. Player Hit
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (nullptr == pObj)
        return;

    LAYER_PLAYER == pObj->GetLayerIdx() && L"Body Collider" == pObj->GetName() ? pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(m_HitInfo)
                                                                               : void();

    // 2. Static Object Hit
    LAYER_STATIC == pObj->GetLayerIdx() ? ChangeState(CUTTERPROJECTILE_STATE::Destroy) : void();

    // 3. 주인과 충돌
    if (_OtherCollider->GetOwner() == m_pAttackPoint)
    {
        m_pOwner->GetScript<CSirKibbleScript>()->ChangeState(SIRKIBBLE_STATE::CutterCatch);

        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CCutterProjectileScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

void CCutterProjectileScript::SaveToLevelFile(FILE* _File)
{
    CMonsterATKScript::SaveToLevelFile(_File);
    fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CCutterProjectileScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterATKScript::LoadFromLevelFile(_File);
    fread(&m_fSpeed, sizeof(float), 1, _File);
}