#include "pch.h"
#include "CKirbyHoveringLanding.h"

CKirbyHoveringLanding::CKirbyHoveringLanding()
{
}

CKirbyHoveringLanding::~CKirbyHoveringLanding()
{
}

void CKirbyHoveringLanding::tick()
{
    if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
    {
        ChangeState(L"DROP_ABILITY");
    }
    else if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"HOVERING_SPIT");
    }
}

void CKirbyHoveringLanding::Enter()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("FlightLanding"), false);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(PLAYERUNIT->GetInitInfo().Speed / 3.f);

    PLAYERFSM->SetHovering(true);
    PLAYERFSM->SetDroppable(true);
}

void CKirbyHoveringLanding::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->SetHovering(false);
    PLAYERFSM->SetDroppable(false);
}