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
    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedLanding"), false);

    PLAYERCTRL->LockJump();
}

void CKirbyStuffedLanding::Exit()
{
    PLAYERCTRL->UnlockJump();
}