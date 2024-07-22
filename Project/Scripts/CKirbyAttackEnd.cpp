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
    PLAY_CURSTATE(AttackEnd)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        PLAY_CURSTATE(Attack)

        // State Change
        if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
        {
            switch (PLAYERFSM->GetCurObjectIdx())
            {
            case ObjectCopyType::CONE: {
                if (PLAYER->CharacterController()->IsGrounded())
                {
                    ChangeState(L"ATTACK_COMBO1");
                }
            }
            break;
            case ObjectCopyType::STAIR:
                break;
            case ObjectCopyType::LIGHT: {
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
            }
            break;
            }
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
    PLAY_CURSTATE(AttackEndEnter)
}

void CKirbyAttackEnd::Exit()
{
    PLAY_CURSTATE(AttackEndExit)
}