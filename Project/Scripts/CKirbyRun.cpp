#include "pch.h"
#include "CKirbyRun.h"

#define RUN_SPEED 10.f

CKirbyRun::CKirbyRun()
{
}

CKirbyRun::~CKirbyRun()
{
}

void CKirbyRun::tick()
{
    // Change State
    if (KEY_TAP(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    else if (KEY_RELEASED_ARROW || KEY_NONE_ARROW)
    {
        ChangeState(L"IDLE");
    }

    // Per State
    PLAY_CURSTATE(Run)
}

void CKirbyRun::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();

    if (KirbyFSM->GetCurObject())
    {
        KirbyFSM->GetCurObject()->RunEnter();
        GetOwner()->Animator()->Play(KIRBYANIM(L"Run"));
    }
    else
    {
        KirbyFSM->GetCurAbility()->RunEnter();
    }
}

void CKirbyRun::Exit()
{
    PLAY_CURSTATE(RunExit)
}