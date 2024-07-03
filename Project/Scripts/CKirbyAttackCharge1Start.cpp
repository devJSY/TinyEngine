#include "pch.h"
#include "CKirbyAttackCharge1Start.h"

CKirbyAttackCharge1Start::CKirbyAttackCharge1Start()
{
}

CKirbyAttackCharge1Start::~CKirbyAttackCharge1Start()
{
}

void CKirbyAttackCharge1Start::tick()
{
    PLAY_CURSTATE(AttackCharge1Start)

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
            if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
            {
                ChangeState(L"ATTACK_CHARGE1_END");
            }
            else
            {
                if (PLAYER->Animator()->IsFinish())
                {
                    ChangeState(L"ATTACK_CHARGE1");
                }
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

void CKirbyAttackCharge1Start::Enter()
{
    PLAY_CURSTATE(AttackCharge1StartEnter)
}

void CKirbyAttackCharge1Start::Exit()
{
    PLAY_CURSTATE(AttackCharge1StartExit)
}
