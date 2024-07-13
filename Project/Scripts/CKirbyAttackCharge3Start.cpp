#include "pch.h"
#include "CKirbyAttackCharge3Start.h"

CKirbyAttackCharge3Start::CKirbyAttackCharge3Start()
{
}

CKirbyAttackCharge3Start::~CKirbyAttackCharge3Start()
{
}

void CKirbyAttackCharge3Start::tick()
{
    PLAY_CURSTATE(AttackCharge3Start)

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
        case AbilityCopyType::RANGER:
            break;
        case AbilityCopyType::SWORD:
        {
            if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"ATTACK_CHARGE3");
            }
        }
        break;
        }
    }
}

void CKirbyAttackCharge3Start::Enter()
{
    PLAY_CURSTATE(AttackCharge3StartEnter)
}

void CKirbyAttackCharge3Start::Exit()
{
    PLAY_CURSTATE(AttackCharge3StartExit)
}
