#include "pch.h"
#include "CKirbySlideStart.h"

CKirbySlideStart::CKirbySlideStart()
{
}

CKirbySlideStart::~CKirbySlideStart()
{
}

void CKirbySlideStart::tick()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideStart();

    // State Change
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL:
    case AbilityCopyType::FIRE:
    case AbilityCopyType::CUTTER: {
        if (PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"SLIDE");
        }
    }
    break;
    case AbilityCopyType::SWORD: {
        if (PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"SLIDE");
        }
    }
    break;
    }
}

void CKirbySlideStart::Enter()
{
    // 애니메이션 재생
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideStartEnter();
    
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbySlideStart::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideStartExit();

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
