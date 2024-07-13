#include "pch.h"
#include "CKirbyJumpAttack.h"

CKirbyJumpAttack::CKirbyJumpAttack()
{
}

CKirbyJumpAttack::~CKirbyJumpAttack()
{
}

void CKirbyJumpAttack::tick()
{
    PLAY_CURSTATE(JumpAttack)

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
            if (GetOwner()->Animator()->IsFinish())
            {
                if (GetOwner()->CharacterController()->IsGrounded())
                {
                    ChangeState(L"LANDING");
                }
                else if (KEY_TAP(KEY_JUMP))
                {
                    ChangeState(L"HOVERING_START");
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

void CKirbyJumpAttack::Enter()
{
    PLAY_CURSTATE(JumpAttackEnter)
}

void CKirbyJumpAttack::Exit()
{
    PLAY_CURSTATE(JumpAttackExit)
}