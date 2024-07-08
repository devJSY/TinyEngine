#include "pch.h"
#include "CKirbyStuffedRun.h"

CKirbyStuffedRun::CKirbyStuffedRun()
{
}

CKirbyStuffedRun::~CKirbyStuffedRun()
{
}

void CKirbyStuffedRun::tick()
{
    // Change State
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
    {
        ChangeState(L"STUFFED_JUMP");
    }
    else if (PLAYERCTRL->GetInput().Length() == 0.f)
    {
        ChangeState(L"STUFFED_IDLE");
    }
}

void CKirbyStuffedRun::Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedRun"));
}

void CKirbyStuffedRun::Exit()
{
}
