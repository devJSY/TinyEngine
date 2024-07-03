#include "pch.h"
#include "CKirbyDodgeLeft2.h"

void CKirbyDodgeLeft2::tick()
{
}

void CKirbyDodgeLeft2::Enter()
{
}

void CKirbyDodgeLeft2::Exit()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDEL");
    }
}

CKirbyDodgeLeft2::CKirbyDodgeLeft2()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeLeft2"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

CKirbyDodgeLeft2::~CKirbyDodgeLeft2()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
