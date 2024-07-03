#include "pch.h"
#include "CKirbyDodgeBack1.h"

CKirbyDodgeBack1::CKirbyDodgeBack1()
{
}

CKirbyDodgeBack1::~CKirbyDodgeBack1()
{
}


void CKirbyDodgeBack1::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"DODGE_BACK2");
    }
}

void CKirbyDodgeBack1::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeBack1"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeBack1::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
