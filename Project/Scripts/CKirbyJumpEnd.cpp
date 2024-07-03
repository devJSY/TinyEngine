#include "pch.h"
#include "CKirbyJumpEnd.h"

CKirbyJumpEnd::CKirbyJumpEnd()
{
}

CKirbyJumpEnd::~CKirbyJumpEnd()
{
}

void CKirbyJumpEnd::tick()
{
    PLAY_CURSTATE(JumpEnd)

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
            if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
        }
        break;
        case AbilityCopyType::FIRE:
            break;
        case AbilityCopyType::RANGER:
            break;
        case AbilityCopyType::SWORD:
            break;
        }
    }
}

void CKirbyJumpEnd::Enter()
{
    PLAY_CURSTATE(JumpEndEnter)
}

void CKirbyJumpEnd::Exit()
{
    PLAY_CURSTATE(JumpEndExit)
}