#include "pch.h"
#include "CMorphoUnit.h"
#include "CBossMgr.h"
#include "CMorphoFSM.h"

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

void CMorphoUnit::tick()
{
    if (m_CurInfo.HP <= 0.f)
    {
        MRPFSM->ChangeStateGroup(MorphoStateGroup::DEMO, L"DEMO_DEATH");
        MRPFSM->SetGlobalState(true);
    }

    if (MRPFSM->GetPhase() == 1)
    {
        if (m_CurInfo.HP <= m_InitInfo.HP * 0.5f)
        {
            MRPFSM->SetPhase(2);
        }
    }
}

UINT CMorphoUnit::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CMorphoUnit::LoadFromLevelFile(FILE* _File)
{
    return 0;
}