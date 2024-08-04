#include "pch.h"
#include "CElfilisA_Idle.h"
#include "CElfilisFSM.h"

CElfilisA_Idle::CElfilisA_Idle()
{
}

CElfilisA_Idle::~CElfilisA_Idle()
{
}

void CElfilisA_Idle::tick()
{
    if (KEY_TAP(KEY::ENTER))
    {
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::AirSmallAtk1, L"AIR_ATKS_STABCOMBO");
        //ELFFSM->ChangeStateGroup_RandState(ElfilisStateGroup::AirMove);
    }
}

void CElfilisA_Idle::Enter_Step()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("WaitAir"));
}

void CElfilisA_Idle::Exit_Step()
{
}