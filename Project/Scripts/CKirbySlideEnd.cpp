#include "pch.h"
#include "CKirbySlideEnd.h"

CKirbySlideEnd::CKirbySlideEnd()
{
}

CKirbySlideEnd::~CKirbySlideEnd()
{
}

void CKirbySlideEnd::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
        {
            ChangeState(L"RUN_START");
        }
        else if (false == PLAYER->CharacterController()->IsGrounded())
        {
            ChangeState(L"JUMP_END");
        }
        else
        {
            ChangeState(L"IDLE");
        }
    }
}

void CKirbySlideEnd::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"SlideEnd"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbySlideEnd::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
