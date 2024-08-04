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
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    if (PLAYERCTRL->IsGround())
    {
        ChangeState(L"STUFFED_LANDING");
    }
}

void CKirbyStuffedJumpFall::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("StuffedFall"));

    PLAYERCTRL->LockJump();
}

void CKirbyStuffedJumpFall::Exit()
{
    PLAYERCTRL->UnlockJump();
}