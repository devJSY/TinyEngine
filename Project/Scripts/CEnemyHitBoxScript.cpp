#include "pch.h"
#include "CEnemyHitBoxScript.h"
#include "CEnemyScript.h"
#include "CPlayerScript.h"

CEnemyHitBoxScript::CEnemyHitBoxScript()
    : CScript(ENEMYHITBOXSCRIPT)
    , m_Enemy(nullptr)
{
}

CEnemyHitBoxScript::~CEnemyHitBoxScript()
{
}

void CEnemyHitBoxScript::begin()
{
    SetEnable(false);
}

void CEnemyHitBoxScript::SetEnable(bool _bEnable)
{
    if (nullptr == BoxCollider2D())
        return;

    BoxCollider2D()->OnEnabled(_bEnable);
}

void CEnemyHitBoxScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (nullptr == m_Enemy || L"Player" != _OtherCollider->GetOwner()->GetName())
        return;

    CPlayerScript* PlayerScript = _OtherCollider->GetOwner()->GetScript<CPlayerScript>();

    Vec3 PlayerPos = _OtherCollider->GetOwner()->Transform()->GetWorldPos();
    Vec3 EnemyPos = m_Enemy->Transform()->GetWorldPos();

    PlayerPos.z = 0;
    EnemyPos.z = 0;

    Vec3 Dir = PlayerPos - EnemyPos;
    Dir.Normalize();

    PlayerScript->TakeHit(m_Enemy->m_ATK, Dir);
}
