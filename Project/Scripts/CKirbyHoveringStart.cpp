#include "pch.h"
#include "CKirbyHoveringStart.h"

CKirbyHoveringStart::CKirbyHoveringStart()
{
}

CKirbyHoveringStart::~CKirbyHoveringStart()
{
}

void CKirbyHoveringStart::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        if (PLAYERCTRL->GetInput().Length() != 0.f || KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
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
    PLAYERFSM->SetHovering(true);
}

void CKirbyHoveringStart::Exit()
{
}