#include "pch.h"
#include "CKirbyStuffedEat.h"

CKirbyStuffedEat::CKirbyStuffedEat()
{
}

CKirbyStuffedEat::~CKirbyStuffedEat()
{
}

void CKirbyStuffedEat::tick()
{
    // Change State
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"GUARD");
    }
}

void CKirbyStuffedEat::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("StuffedLanding"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();

    PLAYERFSM->SetUnstuffReverse(false);
}

void CKirbyStuffedEat::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();

    PLAYERFSM->SetUnstuffReverse(true);
}