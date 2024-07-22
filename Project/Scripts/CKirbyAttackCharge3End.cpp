#include "pch.h"
#include "CKirbyAttackCharge3End.h"

CKirbyAttackCharge3End::CKirbyAttackCharge3End()
{
}

CKirbyAttackCharge3End::~CKirbyAttackCharge3End()
{
}

void CKirbyAttackCharge3End::tick()
{
    PLAY_CURSTATE(AttackCharge3End)

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
        case AbilityCopyType::SWORD:
        {
            if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"IDLE");
            }
        }
            break;
        }
    }
}

void CKirbyAttackCharge3End::Enter()
{
    PLAY_CURSTATE(AttackCharge3EndEnter)
}

void CKirbyAttackCharge3End::Exit()
{
    PLAY_CURSTATE(AttackCharge3EndExit)
}