#include "pch.h"
#include "CKirbyAbility.h"
#include "CPlayerMgr.h"
#include "CFSMScript.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"

CKirbyAbility::CKirbyAbility()
    : m_Charge1Time(0.f)
    , m_Charge2Time(0.f)
    , m_ComboSuccessTime(1.f)
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
    PLAYER->Animator()->Play(ANIMPREFIX(L"Wait"));
}

void CKirbyAbility::RunEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"Run"));
}

void CKirbyAbility::RunStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"RunStart"), false);
}

void CKirbyAbility::JumpEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"JumpEnd"), false);
}

void CKirbyAbility::JumpStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"JumpL"), false);
}

void CKirbyAbility::JumpFallEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"JumpFall"));
}

void CKirbyAbility::LandingEnter()
{
    if (PLAYERFSM->GetLastJump() == LastJumpType::LOW)
    {
        PLAYER->Animator()->Play(ANIMPREFIX(L"LandingSmall"), false);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX(L"Landing"), false);
    }

    PLAYERCTRL->LockJump();
}

void CKirbyAbility::LandingExit()
{
    PLAYERCTRL->UnlockJump();
}

void CKirbyAbility::LandingEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"LandingEnd"), false);

    PLAYERCTRL->LockJump();
}

void CKirbyAbility::LandingEndExit()
{
    PLAYERCTRL->UnlockJump();
}

void CKirbyAbility::SlideEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"Slide"), false);
}

void CKirbyAbility::SlideStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"SlideStart"), false);
}

void CKirbyAbility::SlideEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"SlideEnd"), false);
}

void CKirbyAbility::GuardEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"Guard"), true, false, 1.f, 0.2);
}