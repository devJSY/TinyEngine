#include "pch.h"
#include "CKirbyHovering.h"

CKirbyHovering::CKirbyHovering()
    : m_SavedGravity(0.f)
    , m_SavedSpeed(0.f)
    , m_bFrmEnter(true)
{
}

CKirbyHovering::~CKirbyHovering()
{
}

void CKirbyHovering::tick()
{
    int FrameIdx = PLAYER->Animator()->GetClipFrameIndex();

    if (m_bFrmEnter && FrameIdx == 0)
    {
        GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Hovering.wav", 1, 50.f);

        PLAYERCTRL->ClearVelocityY();
        PLAYERCTRL->AddVelocity(Vec3(0.f, 7.f, 0.f));
        m_bFrmEnter = false;
    }
    else if (!m_bFrmEnter && FrameIdx != 0)
    {
        m_bFrmEnter = true;
    }

    // Change State
    if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
    {
        ChangeState(L"DROP_ABILITY");
    }
    else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"HOVERING_SPIT");
    }
    else if (PLAYERCTRL->IsGround())
    {
        ChangeState(L"HOVERING_LANDING");
    }
    else if (KEY_RELEASED(KEY_JUMP) || (KEY_NONE(KEY_JUMP)))
    {
        ChangeState(L"HOVERING_FALL");
    }
    else if (PLAYERFSM->GetHoveringAccTime() >= PLAYERFSM->GetHoveringLimitTime())
    {
        ChangeState(L"HOVERING_LIMIT");
    }
}

void CKirbyHovering::Enter()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("Flight"), true, false, 2.f);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    m_SavedGravity = PLAYERCTRL->GetGravity();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetGravity(-10.f);
    PLAYERCTRL->SetSpeed(PLAYERUNIT->GetInitInfo().Speed / 3.f);
    PLAYERCTRL->ClearVelocityY();
    PLAYERCTRL->AddVelocity(Vec3(0.f, 7.f, 0.f));

    PLAYERFSM->SetDroppable(true);
    PLAYERFSM->SetHovering(true);
    m_bFrmEnter = true;
}

void CKirbyHovering::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));

    PLAYERCTRL->SetGravity(m_SavedGravity);
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->SetDroppable(false);
    PLAYERFSM->SetHovering(false);
}