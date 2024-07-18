#include "pch.h"
#include "CCutterProjectileScript.h"

#include "CUnitScript.h"

CCutterProjectileScript::CCutterProjectileScript()
    : CMonsterATKScript(CUTTERPROJECTILESCRIPT)
    , m_fSpeed(0.f)
    , m_fAccTime(0.f)
{
}

CCutterProjectileScript::CCutterProjectileScript(const CCutterProjectileScript& _Origin)
    : CMonsterATKScript(_Origin)
    , m_fSpeed(_Origin.m_fSpeed)
    , m_fAccTime(0.f)
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
        m_vOriginPos = Transform()->GetLocalPos();
        Transform()->SetWorldRotation(m_Quat);
    }
    break;
    case CUTTERPROJECTILE_STATE::AttackStop: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
    }
    break;
    case CUTTERPROJECTILE_STATE::AttackBack: {
        Transform()->SetWorldRotation(-m_Quat);
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
    Rigidbody()->SetVelocity(Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_fSpeed);

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
    CMonsterATKScript::SaveToLevelFile(_File);
    fread(&m_fSpeed, sizeof(float), 1, _File);
}