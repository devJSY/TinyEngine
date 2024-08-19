#include "pch.h"
#include "CKirbyAttackCharge2Start.h"

CKirbyAttackCharge2Start::CKirbyAttackCharge2Start()
{
}

CKirbyAttackCharge2Start::~CKirbyAttackCharge2Start()
{
}

void CKirbyAttackCharge2Start::tick()
{
    PLAY_CURSTATE(AttackCharge2Start)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL:
            break;
        case AbilityCopyType::FIRE:
            break;
        case AbilityCopyType::CUTTER:
            break;
        case AbilityCopyType::SWORD: {
            if (PLAYER->Animator()->IsFinish())
            {
                ChangeState(L"ATTACK_CHARGE2");
            }
        }
        break;
        }
    }
}

void CKirbyAttackCharge2Start::Enter()
{
    PLAY_CURSTATE(AttackCharge2StartEnter)
}

void CKirbyAttackCharge2Start::Exit()
{
    PLAY_CURSTATE(AttackCharge2StartExit)
}