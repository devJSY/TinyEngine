#include "pch.h"
#include "CKirbyAttackCharge2SlashEnd.h"

CKirbyAttackCharge2SlashEnd::CKirbyAttackCharge2SlashEnd()
{
}

CKirbyAttackCharge2SlashEnd::~CKirbyAttackCharge2SlashEnd()
{
}

void CKirbyAttackCharge2SlashEnd::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyAttackCharge2SlashEnd::Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"SuperSpinSlashEnd"), false, false, 1.5f);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyAttackCharge2SlashEnd::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}