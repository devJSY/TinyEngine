#include "pch.h"
#include "CKirbyGuard.h"


CKirbyGuard::CKirbyGuard()
{
}

CKirbyGuard::~CKirbyGuard()
{
}
void CKirbyGuard::tick()
{
    if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
    {
        ChangeState(L"DODGE_START");
    }

    else if (KEY_TAP(KEY_JUMP) || KEY_PRESSED(KEY_JUMP))
    {
        ChangeState(L"SLIDE_START");
    }

    else if (KEY_RELEASED(KEY_GUARD) || KEY_NONE(KEY_GUARD))
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyGuard::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"Guard"), true);

    // 방향, 점프, 이동 Lock
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyGuard::Exit()
{
    // 방향, 점프, 이동 Unlock
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

}
