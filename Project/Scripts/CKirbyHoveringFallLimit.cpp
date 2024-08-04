#include "pch.h"
#include "CKirbyHoveringFallLimit.h"

CKirbyHoveringFallLimit::CKirbyHoveringFallLimit()
    : m_SavedGravity(0.f)
    , m_SavedSpeed(0.f)
{
}

CKirbyHoveringFallLimit::~CKirbyHoveringFallLimit()
{
}

void CKirbyHoveringFallLimit::tick()
{
    if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
    {
        ChangeState(L"DROP_ABILITY");
    }
    else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"HOVERING_SPIT");
    }
    else if (PLAYERCTRL->IsGround())
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
    GetOwner()->Animator()->Play(ANIMPREFIX("FlightLimitFall"), true, false, 1.5f);
    CPlayerMgr::SetPlayerFace(FaceType::Frown);
    
    if (PLAYERCTRL->GetVelocity().y > 0.f)
    {
        PLAYERCTRL->ClearVelocityY();
    }
    m_SavedGravity = PLAYERCTRL->GetGravity();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetGravity(-3.f);
    PLAYERCTRL->SetSpeed(7.f);

    PLAYERFSM->SetDroppable(true);
}

void CKirbyHoveringFallLimit::Exit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
    PLAYERCTRL->SetGravity(m_SavedGravity);
    PLAYERCTRL->SetSpeed(m_SavedSpeed);
    PLAYERFSM->SetDroppable(false);
}