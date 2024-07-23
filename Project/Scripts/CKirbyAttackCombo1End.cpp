#include "pch.h"
#include "CKirbyAttackCombo1End.h"

CKirbyAttackCombo1End::CKirbyAttackCombo1End()
{
}

CKirbyAttackCombo1End::~CKirbyAttackCombo1End()
{
}

void CKirbyAttackCombo1End::tick()
{
    PLAY_CURSTATE(AttackCombo1End)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE: {
            if (PLAYER->Animator()->IsFinish())
            {
                ChangeState(L"IDLE");
            }
        }
        break;
        case ObjectCopyType::VENDING_MACHINE:
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

void CKirbyAttackCombo1End::Enter()
{
    PLAY_CURSTATE(AttackCombo1EndEnter)
}

void CKirbyAttackCombo1End::Exit()
{
    PLAY_CURSTATE(AttackCombo1EndExit)
}