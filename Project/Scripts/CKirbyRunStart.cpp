#include "pch.h"
#include "CKirbyRunStart.h"

#include "CKirbyFSM.h"
#include "CPlayerMgr.h"
#include "CKirbyObject.h"
#include "CKirbyAbility.h"

CKirbyRunStart::CKirbyRunStart()
{
}

CKirbyRunStart::~CKirbyRunStart()
{
}

void CKirbyRunStart::tick()
{
}

void CKirbyRunStart::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();

    if (KirbyFSM->GetCurObject())
    {
        KirbyFSM->GetCurObject()->RunEnter();
    }
    else
    {
        // KirbyFSM->GetCurObject()->IdleEnter();
        GetOwner()->Animator()->Play(KIRBYANIM(L"RunStart", false));
    }
}

void CKirbyRunStart::Exit()
{
}