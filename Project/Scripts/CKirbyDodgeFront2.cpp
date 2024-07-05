#include "pch.h"
#include "CKirbyDodgeFront2.h"

CKirbyDodgeFront2::CKirbyDodgeFront2()
{
}

CKirbyDodgeFront2::~CKirbyDodgeFront2()
{
}

void CKirbyDodgeFront2::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyDodgeFront2::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeFront2"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeFront2::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
