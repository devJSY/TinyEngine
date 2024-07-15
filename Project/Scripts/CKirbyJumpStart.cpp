#include "pch.h"
#include "CKirbyJumpStart.h"
#include "CKirbyMoveController.h"

CKirbyJumpStart::CKirbyJumpStart()
    : m_JumpAccTime(0.f)
    , m_MinJumpTime(0.2f)
    , m_MaxJumpTime(0.3f)
    , m_bVelocityCut(false)
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
        case AbilityCopyType::NORMAL:
        case AbilityCopyType::SWORD: {
            /*if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
            else */if (KEY_TAP(KEY_JUMP))
            {
                ChangeState(L"HOVERING_START");
            }
            else if (m_JumpAccTime > m_MaxJumpTime)
            {
                if (KEY_PRESSED(KEY_JUMP))
                {
                    ChangeState(L"JUMP");
                }
                else if (KEY_RELEASED(KEY_JUMP) || KEY_NONE(KEY_JUMP))
                {
                    ChangeState(L"JUMP_FALL");
                }
            }
            if (m_JumpAccTime > m_MinJumpTime && m_bVelocityCut == false && ((KEY_RELEASED(KEY_JUMP) || KEY_NONE(KEY_JUMP))))
            {
                //PLAYERCTRL->SetGravity(m_OriginGravity / 2.f);
                PLAYERCTRL->VelocityCut(2.f);
                m_bVelocityCut = true;
            }

        }
        break;
        case AbilityCopyType::FIRE: {
            /*if (GetOwner()->CharacterController()->IsGrounded())
            {
                ChangeState(L"LANDING");
            }
            else */
            if (KEY_TAP(KEY_JUMP))
            {
                ChangeState(L"HOVERING_START");
            }
            else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
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
            else if (m_JumpAccTime > m_MaxJumpTime)
            {
                if (KEY_PRESSED(KEY_JUMP))
                {
                    ChangeState(L"JUMP");
                }
                else if (KEY_RELEASED(KEY_JUMP) || KEY_NONE(KEY_JUMP))
                {
                    ChangeState(L"JUMP_FALL");
                }
            }

            if (m_JumpAccTime > m_MinJumpTime && m_bVelocityCut == false && ((KEY_RELEASED(KEY_JUMP) || KEY_NONE(KEY_JUMP))))
            {
                // PLAYERCTRL->SetGravity(m_OriginGravity / 2.f);
                PLAYERCTRL->VelocityCut(2.f);
                m_bVelocityCut = true;
            }
        }
            break;
        case AbilityCopyType::RANGER:
            break;
        }
    }
}

void CKirbyJumpStart::Enter()
{
    PLAY_CURSTATE(JumpStartEnter)

    PLAYERCTRL->Jump();
    PLAYERFSM->SetLastJump(LastJumpType::LOW);

    m_JumpAccTime = 0.f;
    m_bVelocityCut = false;
}

void CKirbyJumpStart::Exit()
{
    PLAY_CURSTATE(JumpStartExit)
}
