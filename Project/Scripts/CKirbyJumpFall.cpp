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
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL: 
        {
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
        }
        break;
        case AbilityCopyType::FIRE: 
        {
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
        }
            break;
        case AbilityCopyType::RANGER:
            break;
        case AbilityCopyType::SWORD:
            break;
        }
    }
}

void CKirbyJumpFall::Enter()
{
    PLAY_CURSTATE(JumpFallEnter)
}

void CKirbyJumpFall::Exit()
{
    PLAY_CURSTATE(JumpFallExit)
}