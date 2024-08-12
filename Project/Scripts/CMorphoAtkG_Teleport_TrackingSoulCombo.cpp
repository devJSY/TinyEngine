#include "pch.h"
#include "CMorphoAtkG_Teleport_TrackingSoulCombo.h"
#include "CMorphoFSM.h"

CMorphoAtkG_Teleport_TrackingSoulCombo::CMorphoAtkG_Teleport_TrackingSoulCombo()
{
}

CMorphoAtkG_Teleport_TrackingSoulCombo::~CMorphoAtkG_Teleport_TrackingSoulCombo()
{
}

void CMorphoAtkG_Teleport_TrackingSoulCombo::tick()
{
}

void CMorphoAtkG_Teleport_TrackingSoulCombo::Enter_Step()
{
    MRPFSM->SetPattern(MorphoPatternType::TrackingSoulCombo);
    MRPFSM->ProcPatternStep();
}

void CMorphoAtkG_Teleport_TrackingSoulCombo::Exit_Step()
{
}