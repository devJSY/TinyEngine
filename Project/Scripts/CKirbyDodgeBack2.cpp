#include "pch.h"
#include "CKirbyDodgeBack2.h"

CKirbyDodgeBack2::CKirbyDodgeBack2()
{
}

CKirbyDodgeBack2::~CKirbyDodgeBack2()
{
}

void CKirbyDodgeBack2::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyDodgeBack2::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeBack2"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeBack2::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
