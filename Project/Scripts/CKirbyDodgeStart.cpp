#include "pch.h"
#include "CKirbyDodgeStart.h"



CKirbyDodgeStart::CKirbyDodgeStart()
{
}

CKirbyDodgeStart::~CKirbyDodgeStart()
{
}

void CKirbyDodgeStart::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"DODGE1");
    }
}

void CKirbyDodgeStart::Enter()
{


    // 애니메이션 재생
    //PLAYER->Animator()->Play(ANIMPREFIX("DodgeStart"), false, false, 2.f, 0);
    PLAYER->Animator()->Play(ANIMPREFIX("DodgeStart"), false, false, 2.f);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeStart::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
