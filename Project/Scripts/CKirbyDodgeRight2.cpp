#include "pch.h"
#include "CKirbyDodgeRight2.h"

CKirbyDodgeRight2::CKirbyDodgeRight2()
{
}

CKirbyDodgeRight2::~CKirbyDodgeRight2()
{
}

void CKirbyDodgeRight2::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyDodgeRight2::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeRight2"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeRight2::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
