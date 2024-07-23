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
            else if (m_JumpAccTime > m_MaxJumpTime)
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;
        case ObjectCopyType::VENDING_MACHINE: {
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                UINT LeftCanCount = PLAYERFSM->GetCanCount();

                if (LeftCanCount > 0)
                {
                    ChangeState(L"ATTACK_START");
                }
                else
                {
                    ChangeState(L"ATTACK_FAILED");
                }
            }
            else if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_OBJECT_START");
            }
            else if (m_JumpAccTime > m_MaxJumpTime)
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;

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
            else if (m_JumpAccTime > m_MaxJumpTime)
            {
                ChangeState(L"JUMP_FALL");
            }
        }
            break;
        }

        if (m_JumpAccTime > m_MinJumpTime && m_bVelocityCut == false && ((KEY_RELEASED(KEY_JUMP) || KEY_NONE(KEY_JUMP))))
        {
            PLAYERCTRL->VelocityCut(2.f);
            m_bVelocityCut = true;
        }
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
        case AbilityCopyType::CUTTER: 
            {
                /*if (GetOwner()->CharacterController()->IsGrounded())
                {
                    ChangeState(L"LANDING");
                }
                else */
                if (KEY_TAP(KEY_JUMP))
                {
                    ChangeState(L"HOVERING_START");
                }
                else if (KEY_TAP(KEY_ATK))
                {
                    if (PLAYERFSM->CanBladeAttack())
                    {
                        ChangeState(L"ATTACK");
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
        }
    }
}

void CKirbyJumpStart::Enter()
{
    PLAY_CURSTATE(JumpStartEnter)

    PLAYERCTRL->Jump();
    PLAYERFSM->SetLastJump(LastJumpType::LOW);
    PLAYERFSM->SetDroppable(true);

    m_JumpAccTime = 0.f;
    m_bVelocityCut = false;
}

void CKirbyJumpStart::Exit()
{
    PLAY_CURSTATE(JumpStartExit)

    PLAYERFSM->SetDroppable(false);
}
