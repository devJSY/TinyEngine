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
        if (ELFFSM->IsNearPlayer())
        {
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMAL_L");
        }
        else
        {
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMALTELEPORT_L");
        }
    }
    else
    {
        if (ELFFSM->IsNearPlayer())
        {
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMAL_R");
        }
        else
        {
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMALTELEPORT_R");
        }
    }
}

void CElfilisG_NormalAtk::Exit()
{
}