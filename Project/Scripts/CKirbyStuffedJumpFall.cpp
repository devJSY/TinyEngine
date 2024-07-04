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
    // PLAY_CURSTATE(StuffedJumpFall)

    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"STUFFED_LANDING");
    }
}

void CKirbyStuffedJumpFall::Enter()
{
    // PLAY_CURSTATE(StuffedJumpFall)
    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedFall"));
}

void CKirbyStuffedJumpFall::Exit()
{
    // PLAY_CURSTATE(StuffedJumpFall)
}