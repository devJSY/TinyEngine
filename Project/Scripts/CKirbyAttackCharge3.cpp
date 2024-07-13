#include "pch.h"
#include "CKirbyAttackCharge3.h"

CKirbyAttackCharge3::CKirbyAttackCharge3()
{
}

CKirbyAttackCharge3::~CKirbyAttackCharge3()
{
}

void CKirbyAttackCharge3::tick()
{
    PLAY_CURSTATE(AttackCharge3)

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
            if (PLAYER->Animator()->GetClipFrameIndex() >= 20)
            {
                if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
                {
                    ChangeState(L"ATTACK_CHARGE3_END");
                }
            }
        }
            break;
        }
    }
}

void CKirbyAttackCharge3::Enter()
{
    PLAY_CURSTATE(AttackCharge3Enter)
}

void CKirbyAttackCharge3::Exit()
{
    PLAY_CURSTATE(AttackCharge3Exit)
}