#include "pch.h"
#include "CKirbyLadderExit.h"

CKirbyLadderExit::CKirbyLadderExit()
{
}

CKirbyLadderExit::~CKirbyLadderExit()
{
}


void CKirbyLadderExit::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyLadderExit::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("JumpEnd"), false, false, 1.5f);

    // Lock
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->ClearVelocityY();


    PLAYERCTRL->AddVelocity(PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 7.f);

    PLAYERCTRL->SetFrictionMode(true);
    PLAYERCTRL->SetFriction(1.f);

}

void CKirbyLadderExit::Exit()
{
    // Lock
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();

    PLAYERCTRL->SetFrictionMode(false);
}
