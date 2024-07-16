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
    PLAYER->Animator()->Play(ANIMPREFIX(L"StuffedLanding"), false);

    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
}

void CKirbyStuffedEat::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
}