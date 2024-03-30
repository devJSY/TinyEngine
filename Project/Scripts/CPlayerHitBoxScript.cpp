#include "pch.h"
#include "CPlayerHitBoxScript.h"

#include <Engine\\CPhysics2DMgr.h>

#include "CPlayerScript.h"
#include "CEnemyScript.h"

CPlayerHitBoxScript::CPlayerHitBoxScript()
    : CScript(PLAYERHITBOXSCRIPT)
{
}

CPlayerHitBoxScript::~CPlayerHitBoxScript()
{
}

void CPlayerHitBoxScript::begin()
{
    SetEnable(false);
}

void CPlayerHitBoxScript::SetEnable(bool _bEnable)
{
    if (nullptr == BoxCollider2D())
        return;

    BoxCollider2D()->OnEnabled(_bEnable);
}

void CPlayerHitBoxScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    CEnemyScript* EnemyScript = _OtherCollider->GetOwner()->GetScript<CEnemyScript>();
    if (nullptr == EnemyScript)
        return;

    CPlayerScript* PlayerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();

    Vec3 PlayerPos = CGameManagerScript::GetInset()->GetPlayer()->Transform()->GetWorldPos();
    Vec3 EnemyPos = _OtherCollider->GetOwner()->Transform()->GetWorldPos();

    PlayerPos.z = 0;
    EnemyPos.z = 0;

    Vec3 Dir = EnemyPos - PlayerPos;
    Dir.Normalize();

    EnemyScript->TakeHit(PlayerScript->m_ATK, Dir);
}
