#include "pch.h"
#include "CKirbyJumpFall.h"

CKirbyJumpFall::CKirbyJumpFall()
{
}

CKirbyJumpFall::~CKirbyJumpFall()
{
}

void CKirbyJumpFall::tick()
{
    PLAY_CURSTATE(JumpFall)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE: {
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                ChangeState(L"ATTACK_START");
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_OBJECT_START");
            }
            else if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
        }
        break;
        case ObjectCopyType::STAIR:
            break;
        case ObjectCopyType::LIGHT: {
            if (KEY_TAP(KEY_ATK) && !PLAYERFSM->IsAttackEvent())
            {
                ChangeState(L"ATTACK");
            }
            else if ((KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK)) && PLAYERFSM->IsAttackEvent())
            {
                ChangeState(L"ATTACK_END");
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_OBJECT");
            }
            else if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
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
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                ChangeState(L"ATTACK_CHARGE1_START");
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_ABILITY");
            }
            else if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
            else if (KEY_TAP(KEY_JUMP))
            {
                ChangeState(L"HOVERING_START");
            }
        }
        break;
        case AbilityCopyType::FIRE: 
        {
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                if (KEY_PRESSED_ARROW && KEY_TAP(KEY_ATK))
                {
                    ChangeState(L"BURNING_PRE");
                }
                else
                {
                    ChangeState(L"ATTACK_CHARGE1_START");
                }
            }
            else if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
            else if (KEY_TAP(KEY_JUMP))
            {
                ChangeState(L"HOVERING_START");
            }
        }
            break;
        case AbilityCopyType::RANGER:
            break;
        case AbilityCopyType::SWORD: {
            if (PLAYERFSM->GetSlideComboLevel())
            {
                if (GetOwner()->CharacterController()->IsGrounded())
                {
                    ChangeState(L"LANDING");
                }
                else if ((KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK)) && PLAYERFSM->GetSlideComboLevel() == 1)
                {
                    ChangeState(L"JUMP_ATTACK_START");
                }
            }
            else if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
            else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                if (KEY_TAP(KEY_GUARD) || KEY_PRESSED(KEY_GUARD))
                {
                    ChangeState(L"ATTACK_AIRGUARD_CHARGE_START");
                }
                else
                {
                    ChangeState(L"JUMP_ATTACK_START");
                }
            }
            else if (KEY_TAP(KEY_JUMP))
            {
                ChangeState(L"HOVERING_START");
            }
        }
        break;
        }
    }
}

void CKirbyJumpFall::Enter()
{
    PLAY_CURSTATE(JumpFallEnter)
    PLAYERFSM->SetDroppable(true);
}

void CKirbyJumpFall::Exit()
{
    PLAY_CURSTATE(JumpFallExit)
    PLAYERFSM->SetDroppable(false);
}