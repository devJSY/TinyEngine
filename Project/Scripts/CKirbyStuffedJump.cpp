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
    // PLAY_CURSTATE(StuffedJump)

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
    // PLAY_CURSTATE(StuffedJump)

    //@TODO 점프높이
    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedJump"), false);
}

void CKirbyStuffedJump::Exit()
{
    // PLAY_CURSTATE(StuffedJump)
}