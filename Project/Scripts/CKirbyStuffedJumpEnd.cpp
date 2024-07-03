#include "pch.h"
#include "CKirbyStuffedJumpEnd.h"

CKirbyStuffedJumpEnd::CKirbyStuffedJumpEnd()
{
}

CKirbyStuffedJumpEnd::~CKirbyStuffedJumpEnd()
{
}

void CKirbyStuffedJumpEnd::tick()
{
    // PLAY_CURSTATE(StuffedJumpEnd)

    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"STUFFED_LANDING");
    }
}

void CKirbyStuffedJumpEnd::Enter()
{
    // PLAY_CURSTATE(StuffedJumpEnd)
    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedFall"));
}

void CKirbyStuffedJumpEnd::Exit()
{
    // PLAY_CURSTATE(StuffedJumpEnd)
}