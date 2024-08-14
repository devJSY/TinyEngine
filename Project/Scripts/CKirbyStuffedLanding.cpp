#include "pch.h"
#include "CKirbyStuffedLanding.h"

CKirbyStuffedLanding::CKirbyStuffedLanding()
{
}

CKirbyStuffedLanding::~CKirbyStuffedLanding()
{
}

void CKirbyStuffedLanding::tick()
{
    // Change State
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"STUFFED_IDLE");
    }
}

void CKirbyStuffedLanding::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("StuffedLanding"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    PLAYERCTRL->LockJump();
}

void CKirbyStuffedLanding::Exit()
{
    PLAYERCTRL->UnlockJump();
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
}