#include "pch.h"
#include "CKirbyDodgeStart.h"



CKirbyDodgeStart::CKirbyDodgeStart()
{
}

CKirbyDodgeStart::~CKirbyDodgeStart()
{
}

void CKirbyDodgeStart::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        switch (PLAYERFSM->GetDodgeType())
        {
        case DodgeType::NONE:
            ChangeState(L"IDLE");
            break;
        case DodgeType::FRONT:
            ChangeState(L"DODGE_FRONT1");
            break;
        case DodgeType::RIGHT:
            ChangeState(L"DODGE_RIGHT1");
            break;
        case DodgeType::BACK:
            ChangeState(L"DODGE_BACK1");
            break;
        case DodgeType::LEFT:
            ChangeState(L"DODGE_LEFT1");
            break;
        default:
            break;
        }

    }
}

void CKirbyDodgeStart::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeStart"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyDodgeStart::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
