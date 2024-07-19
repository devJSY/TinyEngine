#include "pch.h"
#include "CKirbyLanding.h"
#include "CKirbyMoveController.h"

CKirbyLanding::CKirbyLanding()
{
}

CKirbyLanding::~CKirbyLanding()
{
}

void CKirbyLanding::tick()
{
    PLAY_CURSTATE(Landing)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE:
        case ObjectCopyType::STAIR:
        case ObjectCopyType::LIGHT: {
            if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"LANDING_END");
            }
        }
        break;
        }
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL:
        case AbilityCopyType::FIRE:
        case AbilityCopyType::RANGER: {
            if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"LANDING_END");
            }
        }
        break;
        case AbilityCopyType::SWORD: {
            if (GetOwner()->Animator()->IsFinish())
            {
                if (PLAYERFSM->GetSlideComboLevel() == 2)
                {
                    ChangeState(L"BACKJUMP");
                }
                else
                {
                    ChangeState(L"LANDING_END");
                }
            }
        }
            break;
        }
    }
}

void CKirbyLanding::Enter()
{
    PLAY_CURSTATE(LandingEnter)
    PLAYERFSM->SetDroppable(true);
}

void CKirbyLanding::Exit()
{
    PLAY_CURSTATE(LandingExit)
    PLAYERFSM->SetDroppable(false);
}
