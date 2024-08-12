#include "pch.h"
#include "CMorphoMoveG_TeleportCombo.h"
#include "CMorphoFSM.h"

CMorphoMoveG_TeleportCombo::CMorphoMoveG_TeleportCombo()
{
}

CMorphoMoveG_TeleportCombo::~CMorphoMoveG_TeleportCombo()
{
}

void CMorphoMoveG_TeleportCombo::tick()
{
}

void CMorphoMoveG_TeleportCombo::Enter_Step()
{
    MRPFSM->SetPattern(MorphoPatternType::TeleportCombo);
    MRPFSM->ProcPatternStep();
}

void CMorphoMoveG_TeleportCombo::Exit_Step()
{
}