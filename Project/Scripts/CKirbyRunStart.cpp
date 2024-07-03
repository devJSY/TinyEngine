#include "pch.h"
#include "CKirbyRunStart.h"

CKirbyRunStart::CKirbyRunStart()
{
}

CKirbyRunStart::~CKirbyRunStart()
{
}

void CKirbyRunStart::tick()
{
    // Change State
    if (KEY_TAP(KEY_ATK))
        ChangeState(L"ATTACK");
    else if (KEY_RELEASED_ARROW || KEY_NONE_ARROW)
        ChangeState(L"IDLE");
    else
    {
        if (GetOwner()->Animator()->IsFinish())
            ChangeState(L"RUN");
    }
}

void CKirbyRunStart::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();

    if (KirbyFSM->GetCurObject())
    {
        KirbyFSM->GetCurObject()->RunEnter();
        GetOwner()->Animator()->Play(KIRBYANIM(L"RunStart", false));
    }
    else
    {
        KirbyFSM->GetCurAbility()->RunStartEnter();
    }
}

void CKirbyRunStart::Exit()
{
}