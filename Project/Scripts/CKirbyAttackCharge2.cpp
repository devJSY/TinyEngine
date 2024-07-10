#include "pch.h"
#include "CKirbyAttackCharge2.h"

CKirbyAttackCharge2::CKirbyAttackCharge2()
{
}

CKirbyAttackCharge2::~CKirbyAttackCharge2()
{
}

void CKirbyAttackCharge2::tick()
{
    PLAY_CURSTATE(AttackCharge2)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL: 
        {
            if (PLAYERFSM->IsStuffed())
            {
                ChangeState(L"STUFFED");
            }
            else if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
            {
                ChangeState(L"ATTACK_CHARGE1_END");
            }
            else if (PLAYERCTRL->GetInput().Length() != 0.f)
            {
                ChangeState(L"ATTACK_CHARGE2_RUN");
            }
        }
        break;
        case AbilityCopyType::FIRE:
            break;
        case AbilityCopyType::RANGER:
            break;
        case AbilityCopyType::SWORD:
            break;
        }
    }
}

void CKirbyAttackCharge2::Enter()
{
    PLAY_CURSTATE(AttackCharge2Enter)
}

void CKirbyAttackCharge2::Exit()
{
    PLAY_CURSTATE(AttackCharge2Exit)
}
