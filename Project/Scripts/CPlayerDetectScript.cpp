#include "pch.h"
#include "CPlayerDetectScript.h"
#include "CEnemyScript.h"

CPlayerDetectScript::CPlayerDetectScript()
    : CScript(PLAYERDETECTSCRIPT)
{
}

CPlayerDetectScript::~CPlayerDetectScript()
{
}

void CPlayerDetectScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
}

void CPlayerDetectScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
}

void CPlayerDetectScript::OnCollisionExit(CCollider2D* _OtherCollider)
{
}

void CPlayerDetectScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (L"Player" != _OtherCollider->GetOwner()->GetName() || nullptr == GetOwner()->GetParent())
        return;

    CEnemyScript* EnemyScript = GetOwner()->GetParent()->GetScript<CEnemyScript>();
    EnemyScript->OnDetectTargetEnter(_OtherCollider->GetOwner());
}

void CPlayerDetectScript::OnTriggerStay(CCollider2D* _OtherCollider)
{
}

void CPlayerDetectScript::OnTriggerExit(CCollider2D* _OtherCollider)
{
    if (L"Player" != _OtherCollider->GetOwner()->GetName() || nullptr == GetOwner()->GetParent())
        return;

    CEnemyScript* EnemyScript = GetOwner()->GetParent()->GetScript<CEnemyScript>();
    EnemyScript->OnDetectTargetExit(_OtherCollider->GetOwner());
}
