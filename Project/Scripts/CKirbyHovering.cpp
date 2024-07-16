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
    else if (GetOwner()->CharacterController()->IsGrounded())
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
    GetOwner()->Animator()->Play(KIRBYANIM(L"Flight"), true, false, 2.f);

    m_SavedGravity = PLAYERCTRL->GetGravity();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetGravity(-10.f);
    PLAYERCTRL->SetSpeed(7.f);
    PLAYERCTRL->ClearVelocityY();
    PLAYERCTRL->AddVelocity(Vec3(0.f, 7.f, 0.f));

    PLAYERFSM->SetDroppable(true);
    m_bFrmEnter = true;
}

void CKirbyHovering::Exit()
{
    PLAYERCTRL->SetGravity(m_SavedGravity);
    PLAYERFSM->SetDroppable(false);
}