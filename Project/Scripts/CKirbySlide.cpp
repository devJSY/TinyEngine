#include "pch.h"
#include "CKirbySlide.h"

CKirbySlide::CKirbySlide()
{
}

CKirbySlide::~CKirbySlide()
{
}

void CKirbySlide::tick()
{
    // 이동은 MoveController에서

    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"SLIDE_END");
    }
}

void CKirbySlide::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"Slide"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbySlide::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
