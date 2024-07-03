#include "pch.h"
#include "CKirbyDodgeLeft1.h"

void CKirbyDodgeLeft1::tick()
{
}

void CKirbyDodgeLeft1::Enter()
{
}

void CKirbyDodgeLeft1::Exit()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"DODGE_LEFT2");
    }
}

CKirbyDodgeLeft1::CKirbyDodgeLeft1()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeLeft1"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

CKirbyDodgeLeft1::~CKirbyDodgeLeft1()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
