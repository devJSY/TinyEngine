#include "pch.h"
#include "CMorphoUnit.h"

CMorphoUnit::CMorphoUnit()
    : CUnitScript(MORPHOUNIT)
{
    UnitInfo MorphoInfo = {
        1000.f, // HP
        1000.f, // MaxHP
        10.f,   // Speed
        10.f,   // Rotation Speed
        10.f,   // JumpPower
        0.f,    // ATK
    };
    SetInitInfo(MorphoInfo);
}

CMorphoUnit::~CMorphoUnit()
{
}

UINT CMorphoUnit::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CMorphoUnit::LoadFromLevelFile(FILE* _File)
{
    return 0;
}