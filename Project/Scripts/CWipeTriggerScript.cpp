#include "pch.h"
#include "CWipeTriggerScript.h"

#include "CUIWipeScript.h"

CWipeTriggerScript::CWipeTriggerScript()
    : CScript(WIPETRIGGERSCRIPT)
{
}


CWipeTriggerScript::CWipeTriggerScript(const CWipeTriggerScript& Origin)
    : CScript(Origin)
{
}

CWipeTriggerScript::~CWipeTriggerScript()
{
}

void CWipeTriggerScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (L"Main Player" == pObj->GetName())
    {
        CGameObject* pStar = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Star");
        nullptr != pStar ? pStar->GetScript<CUIWipeScript>()->ChangeState(WipeState::Minimize) : void();
    }
}

UINT CWipeTriggerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CWipeTriggerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
