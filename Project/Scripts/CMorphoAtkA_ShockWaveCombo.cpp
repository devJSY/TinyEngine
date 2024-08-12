#include "pch.h"
#include "CMorphoAtkA_ShockWaveCombo.h"
#include "CMorphoFSM.h"

CMorphoAtkA_ShockWaveCombo::CMorphoAtkA_ShockWaveCombo()
{
}

CMorphoAtkA_ShockWaveCombo::~CMorphoAtkA_ShockWaveCombo()
{
}

void CMorphoAtkA_ShockWaveCombo::tick()
{
}

void CMorphoAtkA_ShockWaveCombo::Enter_Step()
{
    MRPFSM->SetPattern(MorphoPatternType::ShockWaveCombo);
    MRPFSM->ProcPatternStep();
}

void CMorphoAtkA_ShockWaveCombo::Exit_Step()
{
}