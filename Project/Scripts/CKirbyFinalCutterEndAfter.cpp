#include "pch.h"
#include "CKirbyFinalCutterEndAfter.h"



CKirbyFinalCutterEndAfter::CKirbyFinalCutterEndAfter()
{
}

CKirbyFinalCutterEndAfter::~CKirbyFinalCutterEndAfter()
{
}

void CKirbyFinalCutterEndAfter::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyFinalCutterEndAfter::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("FinalCutterEndAfter"), false, false, 1.5f, 0);

    // 방향, 점프, 이동 Lock
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    // 무적 상태
    PLAYERFSM->SetInvincible(true);

}

void CKirbyFinalCutterEndAfter::Exit()
{
    // 방향, 점프, 이동 Unlock
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    // 무적 상태
    PLAYERFSM->SetInvincible(false);
}
