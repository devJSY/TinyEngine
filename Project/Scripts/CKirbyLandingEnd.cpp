#include "pch.h"
#include "CKirbyLandingEnd.h"

CKirbyLandingEnd::CKirbyLandingEnd()
{
}

CKirbyLandingEnd::~CKirbyLandingEnd()
{
}

void CKirbyLandingEnd::tick()
{
    PLAY_CURSTATE(LandingEnd)

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
                ChangeState(L"IDLE");
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
        case AbilityCopyType::RANGER:
        case AbilityCopyType::SWORD: {
            if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"IDLE");
            }
        }
        break;
        }
    }
}

void CKirbyLandingEnd::Enter()
{
    PLAY_CURSTATE(LandingEndEnter)
    PLAYERFSM->SetDroppable(true);
}

void CKirbyLandingEnd::Exit()
{
    PLAY_CURSTATE(LandingEndExit)
    PLAYERFSM->SetDroppable(false);
}
