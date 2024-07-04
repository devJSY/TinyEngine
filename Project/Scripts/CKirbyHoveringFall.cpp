#include "pch.h"
#include "CKirbyHoveringFall.h"

CKirbyHoveringFall::CKirbyHoveringFall()
{
}

CKirbyHoveringFall::~CKirbyHoveringFall()
{
}

void CKirbyHoveringFall::tick()
{
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"HOVERING_SPIT");
    }
    else if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"HOVERING_LANDING");
    }
    else if (PLAYERCTRL->GetInput().Length() != 0.f || KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
    {
        ChangeState(L"HOVERING");
    }
    else if (PLAYERFSM->GetHoveringAccTime() >= PLAYERFSM->GetHoveringLimitTime())
    {
        ChangeState(L"HOVERING_FALL_LIMIT");
    }
}

void CKirbyHoveringFall::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"FlightFall"));
}

void CKirbyHoveringFall::Exit()
{
}
