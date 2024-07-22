#include "pch.h"
#include "CKirbyAttackFailedEnd.h"

CKirbyAttackFailedEnd::CKirbyAttackFailedEnd()
{
}

CKirbyAttackFailedEnd::~CKirbyAttackFailedEnd()
{
}

void CKirbyAttackFailedEnd::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
        {
            ChangeState(L"ATTACK_FAILED");
        }
        else
        {
            ChangeState(L"IDLE");
        }
    }
}

void CKirbyAttackFailedEnd::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("ShootFailedEnd"), false, false, 2.f, 0);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();
}

void CKirbyAttackFailedEnd::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();

}
