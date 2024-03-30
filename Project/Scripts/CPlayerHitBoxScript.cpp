#include "pch.h"
#include "CPlayerHitBoxScript.h"

#include "CPlayerScript.h"
#include "CEnemyScript.h"

CPlayerHitBoxScript::CPlayerHitBoxScript()
    : CScript(PLAYERHITBOXSCRIPT)
    , m_bEnable(false)
    , m_HitBoxScale(Vec3())
{
}

CPlayerHitBoxScript::~CPlayerHitBoxScript()
{
}

void CPlayerHitBoxScript::begin()
{
    m_HitBoxScale = Transform()->GetRelativeScale();
    Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}

void CPlayerHitBoxScript::SetEnable(bool _bEnable)
{
    m_bEnable = _bEnable;

    if (m_bEnable)
        Transform()->SetRelativeScale(m_HitBoxScale);
    else
        Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}

void CPlayerHitBoxScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    CEnemyScript* EnemyScript = _OtherCollider->GetOwner()->GetScript<CEnemyScript>();
    if (nullptr == EnemyScript)
        return;

    CPlayerScript* PlayerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();

    Vec3 PlayerPos = _OtherCollider->GetOwner()->Transform()->GetWorldPos();
    Vec3 EnemyPos = _OtherCollider->GetOwner()->Transform()->GetWorldPos();

    PlayerPos.z = 0;
    EnemyPos.z = 0;

    Vec3 Dir = PlayerPos - EnemyPos;
    Dir.Normalize();

    EnemyScript->TakeHit(PlayerScript->m_ATK, Dir);
}
