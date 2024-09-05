#include "pch.h"
#include "CKirbyHoveringLimit.h"

CKirbyHoveringLimit::CKirbyHoveringLimit()
    : m_SavedGravity(0.f)
    , m_SavedSpeed(0.f)
{
}

CKirbyHoveringLimit::~CKirbyHoveringLimit()
{
}

void CKirbyHoveringLimit::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Hovering.wav", 1, KIRBY_EFFECTSOUND);
        GetOwner()->Animator()->Play(ANIMPREFIX("FlightLimit"), false, false, 1.5f);
    }


    // Change State
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
    else if (KEY_RELEASED(KEY_JUMP) || (KEY_NONE(KEY_JUMP)))
    {
        ChangeState(L"HOVERING_FALL_LIMIT");
    }
}

void CKirbyHoveringLimit::Enter()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("FlightLimit"), false, false, 1.5f);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));
    CPlayerMgr::SetPlayerFace(FaceType::Frown);

    m_SavedGravity = PLAYERCTRL->GetGravity();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetGravity(-3.f);
    PLAYERCTRL->SetSpeed(PLAYERUNIT->GetInitInfo().Speed / 3.f);

    PLAYERFSM->SetHovering(true);
    PLAYERFSM->SetDroppable(true);
}

void CKirbyHoveringLimit::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->SetGravity(m_SavedGravity);
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->SetHovering(false);
    PLAYERFSM->SetDroppable(false);
}