#include "pch.h"
#include "CKirbyStuffedJump.h"

CKirbyStuffedJump::CKirbyStuffedJump()
    : m_JumpAccTime(0.f)
    , m_MinJumpTime(0.2f)
    , m_MaxJumpTime(0.3f)
{
}

CKirbyStuffedJump::~CKirbyStuffedJump()
{
}

void CKirbyStuffedJump::tick()
{
    m_JumpAccTime += DT;

    // Change State
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    else if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"STUFFED_LANDING");
    }
    else if (m_JumpAccTime > m_MaxJumpTime)
    {
        if (KEY_RELEASED(KEY_JUMP) || KEY_NONE(KEY_JUMP))
        {
            PLAYERFSM->SetLastJump(LastJumpType::HIGH);
            ChangeState(L"STUFFED_JUMP_FALL");
        }
    }

    if (m_JumpAccTime > m_MinJumpTime && m_bVelocityCut == false && ((KEY_RELEASED(KEY_JUMP) || KEY_NONE(KEY_JUMP))))
    {
        PLAYERCTRL->VelocityCut(2.f);
        m_bVelocityCut = true;
    }
}

void CKirbyStuffedJump::Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedJump"), false);

    PLAYERCTRL->Jump();
    PLAYERFSM->SetLastJump(LastJumpType::LOW);

    m_JumpAccTime = 0.f;
    m_bVelocityCut = false;
}

void CKirbyStuffedJump::Exit()
{
}