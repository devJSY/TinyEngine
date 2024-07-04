#include "pch.h"
#include "CKirbyDodgeFront1.h"

CKirbyDodgeFront1::CKirbyDodgeFront1()
{
}

CKirbyDodgeFront1::~CKirbyDodgeFront1()
{
}
void CKirbyDodgeFront1::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"DODGE_FRONT2");
    }
}

void CKirbyDodgeFront1::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeFront1"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeFront1::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

