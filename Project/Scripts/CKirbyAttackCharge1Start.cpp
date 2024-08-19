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
            break;
        case AbilityCopyType::FIRE:
        {
            if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
            {
                ChangeState(L"ATTACK_CHARGE1_END");
            }
            else if (PLAYER->Animator()->IsFinish())
            {
                if (PLAYERCTRL->GetInput().Length() != 0.f)
                {
                    ChangeState(L"ATTACK_CHARGE1_RUN");
                }
                else
                {
                    ChangeState(L"ATTACK_CHARGE1");
                }
            }
        }
        break;
        case AbilityCopyType::CUTTER:
            break;
        case AbilityCopyType::SWORD: {
            if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
            {
                if (PLAYERFSM->GetChargeAccTime() >= 0.5f)
                {
                    ChangeState(L"ATTACK_CHARGE1");
                }
                else
                {
                    ChangeState(L"IDLE");
                }
            }
            else if (KEY_PRESSED(KEY_ATK))
            {
                if (PLAYERFSM->GetChargeAccTime() >= PLAYERFSM->GetCurAbility()->GetCharge1Time())
                {
                     ChangeState(L"ATTACK_CHARGE2_START");
                }
            }
        }
        break;
        }
    }
}

void CKirbyAttackCharge1Start::Enter()
{
    PLAY_CURSTATE(AttackCharge1StartEnter)
    PLAYERFSM->ClearChargeAccTime();
}

void CKirbyAttackCharge1Start::Exit()
{
    PLAY_CURSTATE(AttackCharge1StartExit)
}
