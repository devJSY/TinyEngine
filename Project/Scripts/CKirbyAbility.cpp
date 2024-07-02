#include "pch.h"
#include "CKirbyAbility.h"
#include "CPlayerMgr.h"
#include "CFSMScript.h"

CKirbyAbility::CKirbyAbility()
{
}

CKirbyAbility::~CKirbyAbility()
{
}

void CKirbyAbility::ChangeState(const wstring& _strStateName)
{
    CFSMScript* OwnerFSM = (CFSMScript*)CPlayerMgr::GetPlayerFSM();
    assert(OwnerFSM);

    OwnerFSM->ChangeState(_strStateName);
}

void CKirbyAbility::IdleEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Wait"));
}
