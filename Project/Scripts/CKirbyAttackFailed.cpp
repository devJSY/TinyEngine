#include "pch.h"
#include "CKirbyAttackFailed.h"

CKirbyAttackFailed::CKirbyAttackFailed()
{
}

CKirbyAttackFailed::~CKirbyAttackFailed()
{
}

void CKirbyAttackFailed::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"ATTACK_FAILEDEND");
    }
}

void CKirbyAttackFailed::Enter()
{
    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroDeformVendingMachine\\VendingMachine_AttackFailed.wav", 1, KIRBY_EFFECTSOUND);

    PLAYER->Animator()->Play(ANIMPREFIX("ShootFailed"), false, false, 2.f, 0);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();
}

void CKirbyAttackFailed::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();
}