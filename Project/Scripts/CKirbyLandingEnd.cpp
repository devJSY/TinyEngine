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
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL:
        case AbilityCopyType::FIRE:
        case AbilityCopyType::CUTTER:
        case AbilityCopyType::SWORD:
            break;
        }

        if (GetOwner()->Animator()->IsFinish())
        {
            ChangeState(L"IDLE");
        }
    }
}

void CKirbyLandingEnd::Enter()
{
    PLAY_CURSTATE(LandingEndEnter)
}

void CKirbyLandingEnd::Exit()
{
    PLAY_CURSTATE(LandingEndExit)
}
