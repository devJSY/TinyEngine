#include "pch.h"
#include "CPushOutTriggerScript.h"

#include "CPushOutScript.h"

CPushOutTriggerScript::CPushOutTriggerScript()
    : CScript(PUSHOUTTRIGGERSCRIPT)
    , m_pParent(nullptr)
{
}

CPushOutTriggerScript::CPushOutTriggerScript(const CPushOutTriggerScript& Origin)
    : CScript(Origin)
    , m_pParent(nullptr)
{
}

CPushOutTriggerScript::~CPushOutTriggerScript()
{
}

void CPushOutTriggerScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (L"Main Player" == _OtherCollider->GetOwner()->GetName())
    {
        if (m_pParent != nullptr)
            m_pParent->GetScript<CPushOutScript>()->SetState(PushOutState::MoveDest);
    }
}

void CPushOutTriggerScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

UINT CPushOutTriggerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CPushOutTriggerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}