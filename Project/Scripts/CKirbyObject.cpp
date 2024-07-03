#include "pch.h"
#include "CKirbyObject.h"
#include "CPlayerMgr.h"
#include "CFSMScript.h"

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
