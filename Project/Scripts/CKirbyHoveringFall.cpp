#include "pch.h"
#include "CKirbyHoveringFall.h"

CKirbyHoveringFall::CKirbyHoveringFall()
    : m_SavedGravity(0.f)
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
    else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
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
    GetOwner()->Animator()->Play(ANIMPREFIX("FlightFall"));
    
    if (PLAYERCTRL->GetVelocity().y > 0.f)
    {
        PLAYERCTRL->ClearVelocityY();
    }
    m_SavedGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(-3.f);
}

void CKirbyHoveringFall::Exit()
{
    PLAYERCTRL->SetGravity(m_SavedGravity);
}