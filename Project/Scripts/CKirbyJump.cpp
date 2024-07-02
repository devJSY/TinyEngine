#include "pch.h"
#include "CKirbyJump.h"

CKirbyJump::CKirbyJump()
{
}

CKirbyJump::~CKirbyJump()
{
}

void CKirbyJump::tick()
{
    PLAY_CURSTATE(Jump)

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"JUMP_END");
    }
}

void CKirbyJump::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"JumpEnd"), false);
}

void CKirbyJump::Exit()
{
}