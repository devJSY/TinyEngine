#include "pch.h"
#include "CKirbyDodgeRight1.h"

CKirbyDodgeRight1::CKirbyDodgeRight1()
{
}

CKirbyDodgeRight1::~CKirbyDodgeRight1()
{
}

void CKirbyDodgeRight1::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"DODGE_RIGHT2");
    }
}

void CKirbyDodgeRight1::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeRight1"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeRight1::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
