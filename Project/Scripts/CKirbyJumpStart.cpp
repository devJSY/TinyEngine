#include "pch.h"
#include "CKirbyJumpStart.h"
#include "CKirbyMoveController.h"

CKirbyJumpStart::CKirbyJumpStart()
    : m_bJumpEnd(false)
{
}

CKirbyJumpStart::~CKirbyJumpStart()
{
}

void CKirbyJumpStart::tick()
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
        case AbilityCopyType::NORMAL: 
        {
            if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
            else if (KEY_TAP(KEY_JUMP))
            {
                ChangeState(L"HOVERING_START");
            }
            else if (GetOwner()->Animator()->IsFinish())
            {
                if (KEY_PRESSED(KEY_JUMP) && !m_bJumpEnd)
                {
                    ChangeState(L"JUMP");
                }
                else
                {
                    ChangeState(L"JUMP_FALL");
                }
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

    if (GetOwner()->Animator()->IsPlaying())
    {
        if ((KEY_RELEASED(KEY_JUMP) || KEY_NONE(KEY_JUMP)) && !m_bJumpEnd)
        {
            PLAYERCTRL->ClearVelocityY();
            m_bJumpEnd = true;
        }
    }
}

void CKirbyJumpStart::Enter()
{
    PLAY_CURSTATE(JumpStartEnter)

    m_bJumpEnd = false;
}

void CKirbyJumpStart::Exit()
{
    PLAY_CURSTATE(JumpStartExit)
}
