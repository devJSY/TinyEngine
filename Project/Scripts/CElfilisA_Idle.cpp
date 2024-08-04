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
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::AirToGround, L"AIR_TOGROUND_TELEPORTCOMBO");
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