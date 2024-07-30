#include "pch.h"
#include "CKirbyIdleStart.h"

CKirbyIdleStart::CKirbyIdleStart()
{
}

CKirbyIdleStart::~CKirbyIdleStart()
{
}

void CKirbyIdleStart::tick()
{
    PLAY_CURSTATE(IdleStart)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE:
        case ObjectCopyType::VENDING_MACHINE:
        case ObjectCopyType::LIGHT:
            break;
        }
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::FIRE:
        case AbilityCopyType::NORMAL:
        case AbilityCopyType::CUTTER:
        case AbilityCopyType::SWORD:
            break;
        case AbilityCopyType::SLEEP: {
            if (PLAYER->Animator()->IsFinish())
            {
                ChangeState(L"IDLE");
            }
        }
        break;
        }
    }
}

void CKirbyIdleStart::Enter()
{
    PLAY_CURSTATE(IdleStartEnter)
}

void CKirbyIdleStart::Exit()
{
    PLAY_CURSTATE(IdleStartExit)
}