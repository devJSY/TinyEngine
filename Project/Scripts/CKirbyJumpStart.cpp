#include "pch.h"
#include "CKirbyJumpStart.h"
#include "CKirbyMoveController.h"

CKirbyJumpStart::CKirbyJumpStart()
    : m_JumpAccTime(0.f)
    , m_MinJumpTime(0.2f)
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
            else if (PLAYER->Animator()->IsFinish())
            {
                ChangeState(L"JUMP");
            }
            else if (KEY_RELEASED(KEY_JUMP) || KEY_NONE(KEY_JUMP))
            {
                if (m_JumpAccTime > m_MinJumpTime)
                {
                    ChangeState(L"JUMP_FALL");
                }
                else
                {
                    PLAYERCTRL->SetJumpType(JumpType::AIR);
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

    PLAYERCTRL->SetJumpType(JumpType::UP);

    m_JumpAccTime = 0.f;
}

void CKirbyJumpStart::Exit()
{
    PLAY_CURSTATE(JumpStartExit)

    PLAYERCTRL->SetJumpType(JumpType::NONE);
}
