#include "pch.h"
#include "CKirbyJumpStart.h"
#include "CKirbyMoveController.h"

CKirbyJumpStart::CKirbyJumpStart()
    : m_JumpAccTime(0.f)
    , m_MinJumpTime(0.2f)
    , m_MaxJumpTime(0.3f)
{
}

CKirbyJumpStart::~CKirbyJumpStart()
{
}

void CKirbyJumpStart::tick()
{
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
        case AbilityCopyType::NORMAL: {
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
                if (m_JumpAccTime > m_MinJumpTime && m_bVelocityCut == false)
                {
                    PLAYERCTRL->SetGravity(m_OriginGravity / 4.f);
                    PLAYERCTRL->VelocityCut(4.f);
                    m_bVelocityCut = true;
                }
                else if (m_JumpAccTime > m_MaxJumpTime)
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

    PLAYERCTRL->Jump();

    m_OriginGravity = PLAYERCTRL->GetGravity();
    m_JumpAccTime = 0.f;
    m_bVelocityCut = false;
}

void CKirbyJumpStart::Exit()
{
    PLAY_CURSTATE(JumpStartExit)

    PLAYERCTRL->SetGravity(m_OriginGravity);
}
