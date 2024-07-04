#include "pch.h"
#include "CKirbyHovering.h"

CKirbyHovering::CKirbyHovering()
    : m_bFrmEnter(true)
{
}

CKirbyHovering::~CKirbyHovering()
{
}

void CKirbyHovering::tick()
{
    if (GET_CURCLIP_FRM == 9 && m_bFrmEnter)
    {
        PLAYERCTRL->AddForce(Vec3(0.f, 8.5f, 0.f), AddForceType::VelocityChange);
        m_bFrmEnter = false;
    }
    else if (GET_CURCLIP_FRM == 19 && !m_bFrmEnter)
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
        ChangeState(L"HOVERING_FALL");
    }
    else if (PLAYERFSM->GetHoveringAccTime() >= PLAYERFSM->GetHoveringLimitTime())
    {
        ChangeState(L"HOVERING_LIMIT");
    }
}

void CKirbyHovering::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"Flight"));

    m_bFrmEnter = true;
}

void CKirbyHovering::Exit()
{
}