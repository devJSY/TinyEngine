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
        case AbilityCopyType::NORMAL: {
            if (PLAYERFSM->IsStuffed())
            {
                ChangeState(L"STUFFED");
            }
            else if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
            {
                ChangeState(L"ATTACK_CHARGE1_END");
            }
            else if (PLAYER->Animator()->IsFinish())
            {
                if (PLAYERCTRL->GetInput().Length() != 0.f)
                {
                    ChangeState(L"ATTACK_CHARGE2_RUN");
                }
                else
                {
                    ChangeState(L"ATTACK_CHARGE2");
                }
            }
        }
        break;
        case AbilityCopyType::FIRE: // 상태 없음
            break;
        case AbilityCopyType::RANGER:
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