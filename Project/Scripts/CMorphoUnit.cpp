#include "pch.h"
#include "CMorphoUnit.h"
#include "CBossMgr.h"
#include "CMorphoFSM.h"
#include "CFlowMgr_BossMorpho.h"

CMorphoUnit::CMorphoUnit()
    : CUnitScript(MORPHOUNIT)
{
    UnitInfo MorphoInfo = {
        700.f, // HP
        700.f, // MaxHP
        10.f,  // Speed
        10.f,  // Rotation Speed
        10.f,  // JumpPower
        0.f,   // ATK
    };
    SetInitInfo(MorphoInfo);
}

CMorphoUnit::~CMorphoUnit()
{
}

void CMorphoUnit::tick()
{
    CUnitScript::tick();
    CFlowMgr_BossMorpho* FlowMgr = CBossMgr::GetMorphoFlowMgr();

    // Death
    if (m_CurInfo.HP <= 0.f && FlowMgr && FlowMgr->GetFlowState() < BossLevelFlow::Death)
    {
        MRPFSM->ResetFSM();
        MRPFSM->ChangeStateGroup(MorphoStateGroup::DEMO, L"DEMO_DEATH");
    }

    // Phase 1
    else if (MRPFSM->GetPhase() == 1)
    {
        // Cheat : Phase 1 -> Phase 2
        if ((KEY_PRESSED(KEY::LCTRL) && (KEY_TAP(KEY::ENTER))) || (KEY_TAP(KEY::LCTRL) && (KEY_PRESSED(KEY::ENTER))))
        {
            m_CurInfo.HP = m_InitInfo.HP * 0.5f;
        }

        if (m_CurInfo.HP <= m_InitInfo.HP * 0.5f)
        {
            MRPFSM->ResetFSM();
            MRPFSM->SetPhase(2);
            MRPFSM->ChangeStateGroup(MorphoStateGroup::DEMO, L"DEMO_PHASE2");
        }
    }

    // Phase 2
    else if (MRPFSM->GetPhase() == 2)
    {
        // Cheat : Death
        if ((KEY_PRESSED(KEY::LCTRL) && (KEY_TAP(KEY::ENTER))) || (KEY_TAP(KEY::LCTRL) && (KEY_PRESSED(KEY::ENTER))))
        {
            m_CurInfo.HP = 0.f;
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