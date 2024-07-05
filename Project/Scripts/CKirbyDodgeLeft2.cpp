#include "pch.h"
#include "CKirbyDodgeLeft2.h"

CKirbyDodgeLeft2::CKirbyDodgeLeft2()
{
}

CKirbyDodgeLeft2::~CKirbyDodgeLeft2()
{
}
void CKirbyDodgeLeft2::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyDodgeLeft2::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeLeft2"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeLeft2::Exit()
{

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

