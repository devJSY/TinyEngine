#include "pch.h"
#include "CFlowMgr_BossMorpho.h"
#include "CBossMgr.h"
#include "CMorphoFSM.h"

CFlowMgr_BossMorpho::CFlowMgr_BossMorpho()
    : CLevelFlowMgr(FLOWMGR_BOSSMORPHO)
    , m_FlowState(BossLevelFlow::LevelStart)
{
}

CFlowMgr_BossMorpho::CFlowMgr_BossMorpho(const CFlowMgr_BossMorpho& _Origin)
    : CLevelFlowMgr(_Origin)
    , m_FlowState(BossLevelFlow::LevelStart)
{
}

CFlowMgr_BossMorpho::~CFlowMgr_BossMorpho()
{
}

void CFlowMgr_BossMorpho::begin()
{
    CLevelFlowMgr::begin();
    SetToneMappingParam(true, 1, 0.85f, 0.27f, 1.3f, 1.26f, 1.963f);

    m_Barricade = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Barricade", LAYER_MONSTER);
}

void CFlowMgr_BossMorpho::tick()
{
    CLevelFlowMgr::tick();

    switch (m_FlowState)
    {
    case BossLevelFlow::LevelStart: {
        BOSS->SetActive(false);
        if (m_Barricade)
        {
            m_Barricade->SetActive(false);
        }

        m_FlowState = BossLevelFlow::WaitBoss;
    }
    break;

    case BossLevelFlow::WaitBoss:
        break;
    case BossLevelFlow::Fight:
        break;
    }
}

void CFlowMgr_BossMorpho::TriggerEvent(UINT _Idx)
{
    if (_Idx == 0)
    {
        SpawnMorpho();
        m_FlowState = BossLevelFlow::Fight;
    }
}

void CFlowMgr_BossMorpho::SpawnMorpho()
{
    BOSS->SetActive(true);
    MRPFSM->ChangeStateGroup(MorphoStateGroup::DEMO, L"DEMO_APPEAR");

    if (m_Barricade)
    {
        m_Barricade->SetActive(true);
    }
}

UINT CFlowMgr_BossMorpho::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CFlowMgr_BossMorpho::LoadFromLevelFile(FILE* _File)
{
    return 0;
}