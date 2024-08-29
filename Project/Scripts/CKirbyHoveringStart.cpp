#include "pch.h"
#include "CKirbyHoveringStart.h"

CKirbyHoveringStart::CKirbyHoveringStart()
    : m_SavedGravity(0.f)
    , m_SavedSpeed(0.f)
{
}

CKirbyHoveringStart::~CKirbyHoveringStart()
{
}

void CKirbyHoveringStart::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
        {
            ChangeState(L"HOVERING");
        }
        else
        {
            ChangeState(L"HOVERING_FALL");
        }
    }
}

void CKirbyHoveringStart::Enter()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("FlightStart"), false);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    m_SavedGravity = PLAYERCTRL->GetGravity();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetGravity(-10.f);
    PLAYERCTRL->SetSpeed(5.f);

    PLAYERFSM->SetHovering(true);
    PLAYERFSM->SetDroppable(true);
}

void CKirbyHoveringStart::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));

    PLAYERCTRL->SetGravity(m_SavedGravity);
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->SetHovering(false);
    PLAYERFSM->SetDroppable(false);
}