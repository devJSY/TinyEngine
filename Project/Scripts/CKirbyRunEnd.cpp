#include "pch.h"
#include "CKirbyRunEnd.h"

CKirbyRunEnd::CKirbyRunEnd()
{
}

CKirbyRunEnd::~CKirbyRunEnd()
{
}

void CKirbyRunEnd::tick()
{
    PLAY_CURSTATE(RunEnd);

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
        case AbilityCopyType::CUTTER:
        case AbilityCopyType::SWORD:
            break;
        }
    }
}

void CKirbyRunEnd::Enter()
{
    PLAY_CURSTATE(RunEndEnter);
}

void CKirbyRunEnd::Exit()
{
    PLAY_CURSTATE(RunEndExit);
}