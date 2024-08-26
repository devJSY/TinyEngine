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
    if (KEY_TAP(KEY_ATK))
    {
        ChangeState(L"ATTACK");
    }
    /*else if (PLAYERCTRL->IsGround())
    {
        ChangeState(L"STUFFED_LANDING");
    }*/
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
    PLAYER->Animator()->Play(ANIMPREFIX("StuffedJump"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    PLAYERCTRL->Jump();

    PLAYERFSM->SetLastJump(LastJumpType::LOW);
    PLAYERFSM->SetUnstuffReverse(false);

    m_JumpAccTime = 0.f;
    m_bVelocityCut = false;
}

void CKirbyStuffedJump::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERFSM->SetUnstuffReverse(true);
}