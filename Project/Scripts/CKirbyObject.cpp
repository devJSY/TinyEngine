#include "pch.h"
#include "CKirbyObject.h"
#include "CPlayerMgr.h"
#include "CFSMScript.h"
#include "CKirbyMoveController.h"

CKirbyObject::CKirbyObject()
{
}

CKirbyObject::~CKirbyObject()
{
}

void CKirbyObject::ChangeState(const wstring& _strStateName)
{
    CFSMScript* OwnerFSM = (CFSMScript*)CPlayerMgr::GetPlayerFSM();
    assert(OwnerFSM);

    OwnerFSM->ChangeState(_strStateName);
}

void CKirbyObject::LandingEnter()
{
    PLAYERCTRL->LockJump();
}

void CKirbyObject::LandingExit()
{
    PLAYERCTRL->UnlockJump();
}

void CKirbyObject::LandingEndEnter()
{
    PLAYERCTRL->LockJump();
}

void CKirbyObject::LandingEndExit()
{
    PLAYERCTRL->UnlockJump();
}
