#include "pch.h"
#include "CKirbyJumpStart.h"
#include "CKirbyMoveController.h"

CKirbyJumpStart::CKirbyJumpStart()
    : m_JumpAccTime(0.f)
{
}

CKirbyJumpStart::~CKirbyJumpStart()
{
}

void CKirbyJumpStart::tick()
{
    //@TODO 점프높이 다르게
    m_JumpAccTime += DT;

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
                if (KEY_PRESSED(KEY_JUMP))
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
}

void CKirbyJumpStart::Enter()
{
    PLAY_CURSTATE(JumpStartEnter)

    m_JumpAccTime = 0.f;
}

void CKirbyJumpStart::Exit()
{
    PLAY_CURSTATE(JumpStartExit)
}
