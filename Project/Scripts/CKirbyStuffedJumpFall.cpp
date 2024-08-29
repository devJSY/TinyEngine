#include "pch.h"
#include "CKirbyStuffedJumpFall.h"

CKirbyStuffedJumpFall::CKirbyStuffedJumpFall()
{
}

CKirbyStuffedJumpFall::~CKirbyStuffedJumpFall()
{
}

void CKirbyStuffedJumpFall::tick()
{
    // Change State
    if (KEY_TAP(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    else if (PLAYERCTRL->IsGround())
    {
        ChangeState(L"STUFFED_LANDING");
    }
}

void CKirbyStuffedJumpFall::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("StuffedFall"));
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    PLAYERCTRL->LockJump();

    PLAYERFSM->SetUnstuffReverse(false);
}

void CKirbyStuffedJumpFall::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();

    PLAYERFSM->SetUnstuffReverse(true);
}