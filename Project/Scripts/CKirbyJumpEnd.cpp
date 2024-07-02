#include "pch.h"
#include "CKirbyJumpEnd.h"

CKirbyJumpEnd::CKirbyJumpEnd()
{
}

CKirbyJumpEnd::~CKirbyJumpEnd()
{
}

void CKirbyJumpEnd::tick()
{
    PLAY_CURSTATE(Jump)
}

void CKirbyJumpEnd::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"JumpFall"));
}

void CKirbyJumpEnd::Exit()
{
}