#include "pch.h"
#include "CKirbyAttackEnd.h"

CKirbyAttackEnd::CKirbyAttackEnd()
{
}

CKirbyAttackEnd::~CKirbyAttackEnd()
{
}

void CKirbyAttackEnd::tick()
{
    PLAY_CURSTATE(Attack)

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
        case AbilityCopyType::SWORD: {
            if (PLAYER->Animator()->IsFinish())
            {
                if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
                {
                    ChangeState(L"ATTACK_COMBO1");
                }
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

void CKirbyAttackEnd::Enter()
{
    PLAY_CURSTATE(AttackEnter)
}

void CKirbyAttackEnd::Exit()
{
    PLAY_CURSTATE(AttackExit)
}