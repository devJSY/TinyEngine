#include "pch.h"
#include "CKirbyAttackStart.h"

CKirbyAttackStart::CKirbyAttackStart()
{
}

CKirbyAttackStart::~CKirbyAttackStart()
{
}

void CKirbyAttackStart::tick()
{
    PLAY_CURSTATE(AttackStart)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE: {
            if (PLAYER->Animator()->IsFinish())
            {
                ChangeState(L"ATTACK");
            }
        }
        break;
        case ObjectCopyType::STAIR:
        case ObjectCopyType::LIGHT:
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

void CKirbyAttackStart::Enter()
{
    PLAY_CURSTATE(AttackStartEnter)
}

void CKirbyAttackStart::Exit()
{
    PLAY_CURSTATE(AttackStartExit)
}