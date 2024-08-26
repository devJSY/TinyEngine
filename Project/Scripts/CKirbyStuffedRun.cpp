#include "pch.h"
#include "CKirbyStuffedRun.h"

CKirbyStuffedRun::CKirbyStuffedRun()
{
}

CKirbyStuffedRun::~CKirbyStuffedRun()
{
}

void CKirbyStuffedRun::tick()
{
    // Change State
    if (KEY_TAP(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
    {
        ChangeState(L"STUFFED_JUMP");
    }
    else if (PLAYERCTRL->GetInput().Length() == 0.f)
    {
        ChangeState(L"STUFFED_IDLE");
    }

}

void CKirbyStuffedRun::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("StuffedRun"));
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    PLAYERFSM->SetUnstuffReverse(false);
}

void CKirbyStuffedRun::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERFSM->SetUnstuffReverse(true);
}
