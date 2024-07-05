#include "pch.h"
#include "CKirbyDodgeLeft1.h"

CKirbyDodgeLeft1::CKirbyDodgeLeft1()
{
}

CKirbyDodgeLeft1::~CKirbyDodgeLeft1()
{
}

void CKirbyDodgeLeft1::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"DODGE_LEFT2");
    }
}

void CKirbyDodgeLeft1::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeLeft1"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeLeft1::Exit()
{

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

