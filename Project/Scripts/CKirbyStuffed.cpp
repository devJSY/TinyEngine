#include "pch.h"
#include "CKirbyStuffed.h"

CKirbyStuffed::CKirbyStuffed()
{
}

CKirbyStuffed::~CKirbyStuffed()
{
}

void CKirbyStuffed::tick()
{
    // Change State
    if (PLAYER->Animator()->IsFinish())
    {
        if (PLAYERCTRL->GetInput().Length() != 0.f)
        {
            ChangeState(L"STUFFED_RUN");
        }
        else
        {
            ChangeState(L"STUFFED_IDLE");
        }
    }
}

void CKirbyStuffed::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Stuffed"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();
}

void CKirbyStuffed::Exit()
{
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();
}