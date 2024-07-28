#include "pch.h"
#include "CElfilisG_NormalAtk.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"

CElfilisG_NormalAtk::CElfilisG_NormalAtk()
{
}

CElfilisG_NormalAtk::~CElfilisG_NormalAtk()
{
}

void CElfilisG_NormalAtk::tick()
{
}

void CElfilisG_NormalAtk::Enter()
{
    if (ELFFSM->GetPhase() == 1)
    {
        ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundAtk, L"GROUND_ATK_NORMAL_L");
    }
    else
    {
        ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundAtk, L"GROUND_ATK_NORMAL_R");
    }
}

void CElfilisG_NormalAtk::Exit()
{
}