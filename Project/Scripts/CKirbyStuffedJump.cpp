#include "pch.h"
#include "CKirbyStuffedJump.h"

CKirbyStuffedJump::CKirbyStuffedJump()
{
}

CKirbyStuffedJump::~CKirbyStuffedJump()
{
}

void CKirbyStuffedJump::tick()
{
    // Change State
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    else if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"STUFFED_LANDING");
    }
    else if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"STUFFED_JUMP_FALL");
    }
}

void CKirbyStuffedJump::Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedJump"), false);

    PLAYERCTRL->Jump();
}

void CKirbyStuffedJump::Exit()
{
}