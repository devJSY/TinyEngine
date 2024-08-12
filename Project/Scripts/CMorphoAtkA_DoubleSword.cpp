#include "pch.h"
#include "CMorphoAtkA_DoubleSword.h"
#include "CMorphoFSM.h"

CMorphoAtkA_DoubleSword::CMorphoAtkA_DoubleSword()
{
}

CMorphoAtkA_DoubleSword::~CMorphoAtkA_DoubleSword()
{
}

void CMorphoAtkA_DoubleSword::tick()
{
}

void CMorphoAtkA_DoubleSword::Enter_Step()
{
    MRPFSM->SetPattern(MorphoPatternType::DoubleSword);
    MRPFSM->ProcPatternStep();
}

void CMorphoAtkA_DoubleSword::Exit_Step()
{
}