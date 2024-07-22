#include "pch.h"
#include "CKirbyAttack.h"

CKirbyAttack::CKirbyAttack()
{
}

CKirbyAttack::~CKirbyAttack()
{
}

void CKirbyAttack::tick()
{
    PLAY_CURSTATE(Attack)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE: {
            if (PLAYER->Animator()->IsFinish())
            {
                ChangeState(L"ATTACK_END");
            }
        }
            break;
        case ObjectCopyType::VENDING_MACHINE:
            break;
        case ObjectCopyType::LIGHT:
            if (PLAYER->Animator()->IsFinish())
            {
                if (PLAYER->CharacterController()->IsGrounded())
                {
                    ChangeState(L"IDLE");
                }
                else
                {
                    ChangeState(L"JUMP_FALL");
                }
            }
        break;
        }
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL: {
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
        case AbilityCopyType::FIRE: // 상태 없음
            break;
        case AbilityCopyType::CUTTER:
        {
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
        case AbilityCopyType::SWORD: {
            if (PLAYER->Animator()->IsFinish())
            {
                if (KEY_TAP(KEY_ATK))
                {
                    ChangeState(L"ATTACK_COMBO1");
                }
                else if (KEY_PRESSED(KEY_ATK))
                {
                    ChangeState(L"ATTACK_CHARGE1_START");
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

void CKirbyAttack::Enter()
{
    PLAY_CURSTATE(AttackEnter)
}

void CKirbyAttack::Exit()
{
    PLAY_CURSTATE(AttackExit)
}