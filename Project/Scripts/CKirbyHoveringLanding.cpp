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
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"HOVERING_SPIT");
    }
}

void CKirbyHoveringLanding::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"FlightLanding"), false);
}

void CKirbyHoveringLanding::Exit()
{
}