#include "pch.h"
#include "CKirbyAbility.h"
#include "CPlayerMgr.h"
#include "CFSMScript.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"

CKirbyAbility::CKirbyAbility()
    : m_Charge1Time(0.f)
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
    PLAYER->Animator()->Play(KIRBYANIM(L"Wait"));
}

void CKirbyAbility::RunEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Run"));
}

void CKirbyAbility::RunStartEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"RunStart"), false);
}

void CKirbyAbility::JumpEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"JumpEnd"), false);
}

void CKirbyAbility::JumpStartEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"JumpL"), false);
}

void CKirbyAbility::JumpFallEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"JumpFall"));
}

void CKirbyAbility::LandingEnter()
{
    if (PLAYERFSM->GetLastJump() == LastJumpType::LOW)
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"LandingSmall"), false);
    }
    else
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"Landing"), false);
    }

    PLAYERCTRL->LockJump();
}

void CKirbyAbility::LandingExit()
{
    PLAYERCTRL->UnlockJump();
}

void CKirbyAbility::LandingEndEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"LandingEnd"), false);

    PLAYERCTRL->LockJump();
}

void CKirbyAbility::LandingEndExit()
{
    PLAYERCTRL->UnlockJump();
}
