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
        case ObjectCopyType::VENDING_MACHINE: {
            if (PLAYERCTRL->GetInputWorld().Length() != 0.f)
            {
                ChangeState(L"RUN");
            }
            else if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"IDLE");
            }
        }
        break;
        case ObjectCopyType::LIGHT: {
            if (KEY_TAP(KEY_ATK) && !PLAYERFSM->IsAttackEvent())
            {
                ChangeState(L"ATTACK");
            }
            else if ((KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK)) && PLAYERFSM->IsAttackEvent())
            {
                ChangeState(L"ATTACK_END");
            }
            else if (GetOwner()->Animator()->IsFinish())
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
    PLAYERFSM->SetDroppable(true);
}

void CKirbyLandingEnd::Exit()
{
    PLAY_CURSTATE(LandingEndExit)
    PLAYERFSM->SetDroppable(false);
}
