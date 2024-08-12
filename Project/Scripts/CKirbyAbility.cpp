#include "pch.h"
#include "CKirbyAbility.h"

CKirbyAbility::CKirbyAbility()
    : m_Charge1Time(0.f)
    , m_Charge2Time(0.f)
    , m_ComboSuccessTime(1.f)
    , m_Hat(nullptr)
    , m_Weapon(nullptr)
{
}

CKirbyAbility::CKirbyAbility(const CKirbyAbility& _Origin)
    : m_Charge1Time(_Origin.m_Charge1Time)
    , m_Charge2Time(_Origin.m_Charge1Time)
    , m_ComboSuccessTime(_Origin.m_ComboSuccessTime)
    , m_vecWaitingAnim(_Origin.m_vecWaitingAnim)
    , m_Hat(nullptr)
    , m_Weapon(nullptr)
{

}

CKirbyAbility::~CKirbyAbility()
{
}

void CKirbyAbility::ChangeState(const wstring& _strStateName)
{
    CFSMScript* OwnerFSM = (CFSMScript*)CPlayerMgr::GetPlayerFSM();
    assert(OwnerFSM);

    OwnerFSM->ChangeState(_strStateName);
}

void CKirbyAbility::IdleEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Wait"));
}

void CKirbyAbility::IdleExit()
{
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
}

void CKirbyAbility::RunEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Run"));
}

void CKirbyAbility::RunStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("RunStart"), false);
}

void CKirbyAbility::JumpEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("JumpEnd"), false);
}

void CKirbyAbility::JumpStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("JumpL"), false);
}

void CKirbyAbility::JumpFallEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("JumpFall"));
}

void CKirbyAbility::LandingEnter()
{
    if (PLAYERFSM->GetLastJump() == LastJumpType::LOW)
    {
        PLAYER->Animator()->Play(ANIMPREFIX("LandingSmall"), false, false, 1.5f);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX("Landing"), false, false, 1.5f);
    }

    PLAYERCTRL->LockJump();
}

void CKirbyAbility::LandingExit()
{
    PLAYERCTRL->UnlockJump();
}

void CKirbyAbility::LandingEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("LandingEnd"), false);

    PLAYERCTRL->LockJump();
}

void CKirbyAbility::LandingEndExit()
{
    PLAYERCTRL->UnlockJump();
}

void CKirbyAbility::SlideEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Slide"), false);
}

void CKirbyAbility::SlideStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SlideStart"), false);
}

void CKirbyAbility::SlideEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SlideEnd"), false);
}

void CKirbyAbility::GuardEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Guard"), true, false, 1.f, 0.2);
}