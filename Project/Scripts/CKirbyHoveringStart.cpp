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

    //PLAYERCTRL->AddForce(Vec3(0.f, 5.f, 0.f), AddForceType::VelocityChange);
    PLAYERFSM->SetHovering(true);
}

void CKirbyHoveringStart::Exit()
{
}