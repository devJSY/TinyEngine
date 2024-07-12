#include "pch.h"
#include "CKirbyHoveringStart.h"

CKirbyHoveringStart::CKirbyHoveringStart()
    : m_SavedGravity(0.f)
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
    GetOwner()->Animator()->Play(KIRBYANIM(L"FlightStart"), false);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    m_SavedGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(-10.f);

    PLAYERFSM->SetHovering(true);
}

void CKirbyHoveringStart::Exit()
{
    PLAYERCTRL->SetGravity(m_SavedGravity);
}