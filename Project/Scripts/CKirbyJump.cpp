#include "pch.h"
#include "CKirbyJump.h"

CKirbyJump::CKirbyJump()
{
}

CKirbyJump::~CKirbyJump()
{
}

void CKirbyJump::tick()
{
    PLAY_CURSTATE(Jump)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
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
            else if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
            else if (KEY_TAP(KEY_JUMP))
            {
                ChangeState(L"HOVERING_START");
            }
            else if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;

        case AbilityCopyType::FIRE:
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                if (KEY_PRESSED_ARROW)
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
            else if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"JUMP_FALL");
            }

            break;

        case AbilityCopyType::RANGER:
            break;
        case AbilityCopyType::SWORD: {
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
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
            else if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
            else if (KEY_TAP(KEY_JUMP))
            {
                ChangeState(L"HOVERING_START");
            }
            else if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;
        }
    }
}

void CKirbyJump::Enter()
{
    PLAY_CURSTATE(JumpEnter)
    PLAYERFSM->SetLastJump(LastJumpType::HIGH);
}

void CKirbyJump::Exit()
{
    PLAY_CURSTATE(JumpExit)
}