#include "pch.h"
#include "CKirbyHoveringLimit.h"

CKirbyHoveringLimit::CKirbyHoveringLimit()
    : m_bFrmEnter(true)
{
}

CKirbyHoveringLimit::~CKirbyHoveringLimit()
{
}

void CKirbyHoveringLimit::tick()
{
    if (GET_CURCLIP_FRM == 7 && m_bFrmEnter)
    {
        //PLAYERCTRL->AddForce(Vec3(0.f, 3.f, 0.f), AddForceType::VelocityChange);
        m_bFrmEnter = false;
    }
    else if (GET_CURCLIP_FRM == 12 && !m_bFrmEnter)
    {
        m_bFrmEnter = true;
    }

    // Change State
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"HOVERING_SPIT");
    }
    else if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"HOVERING_LANDING");
    }
    else if (KEY_RELEASED(KEY_JUMP) || (KEY_NONE(KEY_JUMP)))
    {
        ChangeState(L"HOVERING_FALL_LIMIT");
    }
}

void CKirbyHoveringLimit::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"FlightLimit"));

    PLAYERCTRL->AddForce(Vec3(0.f, 3.f, 0.f), AddForceType::VelocityChange);
    m_bFrmEnter = true;
}

void CKirbyHoveringLimit::Exit()
{
}