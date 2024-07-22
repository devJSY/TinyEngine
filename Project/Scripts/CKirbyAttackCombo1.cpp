#include "pch.h"
#include "CKirbyAttackCombo1.h"

CKirbyAttackCombo1::CKirbyAttackCombo1()
{
}

CKirbyAttackCombo1::~CKirbyAttackCombo1()
{
}

void CKirbyAttackCombo1::tick()
{
    PLAY_CURSTATE(AttackCombo1)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE: {
            if (PLAYER->Animator()->IsFinish())
            {
                ChangeState(L"ATTACK_COMBO1_END");
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
            break;
        case AbilityCopyType::FIRE:
            break;
        case AbilityCopyType::CUTTER:
            break;
        case AbilityCopyType::SWORD: {
            if (PLAYER->Animator()->IsFinish())
            {
                if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
                {
                    ChangeState(L"ATTACK_COMBO2");
                }
                // else if (KEY_PRESSED(KEY_ATK))
                //{
                //     ChangeState(L"ATTACK_CHARGE1_START");
                // }
                else if (PLAYERCTRL->GetInput().Length() != 0.f)
                {
                    ChangeState(L"RUN_START");
                }
                else
                {
                    ChangeState(L"IDLE");
                }
            }
        }
        break;
        }
    }
}

void CKirbyAttackCombo1::Enter()
{
    PLAY_CURSTATE(AttackCombo1Enter)
}

void CKirbyAttackCombo1::Exit()
{
    PLAY_CURSTATE(AttackCombo1Exit)
}