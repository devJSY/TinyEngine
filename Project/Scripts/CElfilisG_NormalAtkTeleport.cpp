#include "pch.h"
#include "CElfilisG_NormalAtkTeleport.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"

CElfilisG_NormalAtkTeleport::CElfilisG_NormalAtkTeleport()
{
}

CElfilisG_NormalAtkTeleport::~CElfilisG_NormalAtkTeleport()
{
}

void CElfilisG_NormalAtkTeleport::tick()
{
}

void CElfilisG_NormalAtkTeleport::Enter()
{
    if (ELFFSM->GetPhase() == 1)
    {
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundMoveAtk, L"GROUND_MOVEATK_NORMALTELEPORT_L");
    }
    else
    {
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundMoveAtk, L"GROUND_MOVEATK_NORMALTELEPORT_R");
    }
}

void CElfilisG_NormalAtkTeleport::Exit()
{
}
