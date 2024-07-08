#include "pch.h"
#include "CKirbyStuffedIdle.h"

CKirbyStuffedIdle::CKirbyStuffedIdle()
{
}

CKirbyStuffedIdle::~CKirbyStuffedIdle()
{
}

void CKirbyStuffedIdle::tick()
{
    // Change State
    if (KEY_TAP(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
    {
        ChangeState(L"STUFFED_JUMP");
    }
    else if (PLAYERCTRL->GetInput().Length() != 0.f)
    {
        ChangeState(L"STUFFED_RUN");
    }
}

void CKirbyStuffedIdle::Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedWait"));
}

void CKirbyStuffedIdle::Exit()
{
}