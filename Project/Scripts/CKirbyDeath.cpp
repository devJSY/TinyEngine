#include "pch.h"
#include "CKirbyDeath.h"

CKirbyDeath::CKirbyDeath()
{
}

CKirbyDeath::~CKirbyDeath()
{
}

void CKirbyDeath::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"DEATH_WAIT");
    }
}

void CKirbyDeath::Enter()
{
    GetOwner()->Animator()->Play(L"Death", false);

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
}

void CKirbyDeath::Exit()
{
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
}