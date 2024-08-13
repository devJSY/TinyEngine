#include "pch.h"
#include "CFlowMgr_BossElfilis.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"

CFlowMgr_BossElfilis::CFlowMgr_BossElfilis()
    : CLevelFlowMgr(FLOWMGR_BOSSELFILIS)
    , m_FlowState(BossLevelFlow::LevelStart)
    , m_LevelEnterWall(nullptr)
{
}

CFlowMgr_BossElfilis::CFlowMgr_BossElfilis(const CFlowMgr_BossElfilis& _Origin)
    : CLevelFlowMgr(_Origin)
{
}

CFlowMgr_BossElfilis::~CFlowMgr_BossElfilis()
{
}

void CFlowMgr_BossElfilis::begin()
{
    CLevelFlowMgr::begin();

    m_LevelEnterWall = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"LevelEnterWall", LAYER_MONSTER);
}

void CFlowMgr_BossElfilis::tick()
{
    CLevelFlowMgr::tick();

    switch (m_FlowState)
    {
    case BossLevelFlow::LevelStart: {
        BOSS->SetActive(false);
        if (m_LevelEnterWall)
        {
            m_LevelEnterWall->SetActive(false);
        }

        m_FlowState = BossLevelFlow::WaitBoss;
    }
    break;

    case BossLevelFlow::WaitBoss:
        break;
    case BossLevelFlow::SpawnBoss: {
        SpawnElfilis();
        m_FlowState = BossLevelFlow::Fight;
    }
    break;
    case BossLevelFlow::Fight:
        break;
    }
}

void CFlowMgr_BossElfilis::TriggerEvent(UINT _Idx)
{
    if (_Idx == 0)
    {
        m_FlowState = BossLevelFlow::SpawnBoss;
    }
}

void CFlowMgr_BossElfilis::SpawnElfilis()
{
    BOSS->SetActive(true);
    ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_APPEAR1");

    if (m_LevelEnterWall)
    {
        m_LevelEnterWall->SetActive(true);
    }
}

UINT CFlowMgr_BossElfilis::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CFlowMgr_BossElfilis::LoadFromLevelFile(FILE* _File)
{
    return 0;
}