#include "pch.h"
#include "CKirbyHoveringFallLimit.h"

CKirbyHoveringFallLimit::CKirbyHoveringFallLimit()
    : m_SavedGravity(0.f)
{
}

CKirbyHoveringFallLimit::~CKirbyHoveringFallLimit()
{
}

void CKirbyHoveringFallLimit::tick()
{
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"HOVERING_SPIT");
    }
    else if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"HOVERING_LANDING");
    }
    else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
    {
        ChangeState(L"HOVERING_LIMIT");
    }
}

void CKirbyHoveringFallLimit::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"FlightFallLimit"));

    m_SavedGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(-5.f);
}

void CKirbyHoveringFallLimit::Exit()
{
    PLAYERCTRL->SetGravity(m_SavedGravity);
}