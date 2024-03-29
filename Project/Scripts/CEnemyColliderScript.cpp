#include "pch.h"
#include "CEnemyColliderScript.h"
#include "CEnemyScript.h"

CEnemyColliderScript::CEnemyColliderScript()
    : CScript(ENEMYCOLLIDERSCRIPT)
{
}

CEnemyColliderScript::~CEnemyColliderScript()
{
}

void CEnemyColliderScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
}

void CEnemyColliderScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
}

void CEnemyColliderScript::OnCollisionExit(CCollider2D* _OtherCollider)
{
}

void CEnemyColliderScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (L"Player" != _OtherCollider->GetOwner()->GetName() || nullptr == GetOwner()->GetParent())
        return;

    CEnemyScript* EnemyScript = GetOwner()->GetParent()->GetScript<CEnemyScript>();
    EnemyScript->OnDetectTargetEnter(_OtherCollider->GetOwner());
}

void CEnemyColliderScript::OnTriggerStay(CCollider2D* _OtherCollider)
{
}

void CEnemyColliderScript::OnTriggerExit(CCollider2D* _OtherCollider)
{
    if (L"Player" != _OtherCollider->GetOwner()->GetName() || nullptr == GetOwner()->GetParent())
        return;

    CEnemyScript* EnemyScript = GetOwner()->GetParent()->GetScript<CEnemyScript>();
    EnemyScript->OnDetectTargetExit(_OtherCollider->GetOwner());
}
