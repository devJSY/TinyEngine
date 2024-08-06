#include "pch.h"
#include "CPushOutColliderScript.h"

#include "CPushOutScript.h"

CPushOutColliderScript::CPushOutColliderScript()
    : CScript(PUSHOUTCOLLIDERSCRIPT)
    , m_pParent(nullptr)
{
}

CPushOutColliderScript::CPushOutColliderScript(const CPushOutColliderScript& Origin)
    : CScript(Origin)
    , m_pParent(nullptr)
{
}

CPushOutColliderScript::~CPushOutColliderScript()
{
}

void CPushOutColliderScript::OnTriggerStay(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Body Collider" == pObj->GetName())
    {
        GetOwner()->GetParent()->GetScript<CPushOutScript>()->SetPlayer(pObj->GetParent());
    }
}

void CPushOutColliderScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Body Collider" == pObj->GetName())
    {
        GetOwner()->GetParent()->GetScript<CPushOutScript>()->SetPlayer(nullptr);
    }
}

UINT CPushOutColliderScript::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CPushOutColliderScript::LoadFromLevelFile(FILE* _File)
{
    return 0;
}