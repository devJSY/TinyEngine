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
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideEnd();

    // State Change
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL:
    case AbilityCopyType::FIRE:
    case AbilityCopyType::CUTTER: {
        if (PLAYER->Animator()->IsFinish())
        {
            if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
            {
                ChangeState(L"RUN_START");
            }
            else if (false == PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
            else
            {
                ChangeState(L"IDLE");
            }
        }
    }
    break;
    case AbilityCopyType::SWORD: {
        if (PLAYER->Animator()->IsFinish())
        {
            if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
            {
                ChangeState(L"RUN_START");
            }
            else if (false == PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
            else
            {
                ChangeState(L"IDLE");
            }
        }
    }
    break;
    }
}

void CKirbySlideEnd::Enter()
{
    // 애니메이션 재생
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideEndEnter();

    PLAYERCTRL->SetFrictionMode(true);
    PLAYERCTRL->SetFriction(50.f);
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERFSM->SetSlideComboLevel(0);
}

void CKirbySlideEnd::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideEndExit();

    PLAYERCTRL->SetVelocity(Vec3());
    PLAYERCTRL->SetFrictionMode(false);
    PLAYERCTRL->SetFriction(0.f);
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
