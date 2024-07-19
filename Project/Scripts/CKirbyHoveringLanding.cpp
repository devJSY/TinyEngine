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

    PLAYERCTRL->LockJump();
    PLAYERFSM->SetDroppable(true);
}

void CKirbyHoveringLanding::Exit()
{
    PLAYERCTRL->UnlockJump();
    PLAYERFSM->SetDroppable(false);
}