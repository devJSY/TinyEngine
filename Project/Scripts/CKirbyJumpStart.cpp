#include "pch.h"
#include "CKirbyJumpStart.h"

CKirbyJumpStart::CKirbyJumpStart()
{
}

CKirbyJumpStart::~CKirbyJumpStart()
{
}

void CKirbyJumpStart::tick()
{
    PLAY_CURSTATE(Jump)
}

void CKirbyJumpStart::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"Jump"), false);
    // SetGround(false)
}

void CKirbyJumpStart::Exit()
{
}
