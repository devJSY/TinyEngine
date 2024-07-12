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
        case AbilityCopyType::FIRE: {
            if (PLAYER->Animator()->IsFinish())
            {
                if (PLAYERCTRL->GetInput().Length() != 0.f)
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
        case AbilityCopyType::RANGER:
            break;
        case AbilityCopyType::SWORD:
            break;
        }
    }
}

void CKirbyAttackEnd::Enter()
{
}

void CKirbyAttackEnd::Exit()
{
}