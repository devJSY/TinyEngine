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
        PLAYERFSM->SetGlobalState(false);
        ChangeState(L"DeatWait");
    }
}

void CKirbyDeath::Enter()
{
    PLAYERFSM->SetGlobalState(true);
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