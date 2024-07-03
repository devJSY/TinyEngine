#include "pch.h"
#include "CKirbyJumpStart.h"
#include "CKirbyMoveController.h"

CKirbyJumpStart::CKirbyJumpStart()
{
}

CKirbyJumpStart::~CKirbyJumpStart()
{
}

void CKirbyJumpStart::tick()
{
    PLAY_CURSTATE(Jump)

    if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"LANDING");
    }
    else if (PLAYERCTRL->GetJump() == JumpType::DOWN)
    {
        ChangeState(L"JUMP");
    }
}

void CKirbyJumpStart::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"Jump"), false);
    // SetGround(false)
}

void CKirbyJumpStart::Exit()
{
}
