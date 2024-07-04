#include "pch.h"
#include "CKirbyHoveringLimit.h"

CKirbyHoveringLimit::CKirbyHoveringLimit()
{
}

CKirbyHoveringLimit::~CKirbyHoveringLimit()
{
}

void CKirbyHoveringLimit::tick()
{
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"HOVERING_SPIT");
    }
    else if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"HOVERING_LANDING");
    }
    else if (PLAYERCTRL->GetInput().Length() == 0.f && (KEY_RELEASED(KEY_JUMP) || (KEY_NONE(KEY_JUMP))))
    {
        ChangeState(L"HOVERING_FALL_LIMIT");
    }
}

void CKirbyHoveringLimit::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"FlightLimit"));
}

void CKirbyHoveringLimit::Exit()
{
}