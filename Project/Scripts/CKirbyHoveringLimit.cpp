#include "pch.h"
#include "CKirbyHoveringLimit.h"

CKirbyHoveringLimit::CKirbyHoveringLimit()
    : m_SavedGravity(0.f)
{
}

CKirbyHoveringLimit::~CKirbyHoveringLimit()
{
}

void CKirbyHoveringLimit::tick()
{
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
    GetOwner()->Animator()->Play(ANIMPREFIX("FlightLimit"), true, false, 1.5f);
    CPlayerMgr::SetPlayerFace(FaceType::Frown);

    m_SavedGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(-3.f);
}

void CKirbyHoveringLimit::Exit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
    PLAYERCTRL->SetGravity(m_SavedGravity);
}