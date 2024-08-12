#include "pch.h"
#include "CFlowMgr_BossElfilis.h"
#include "CBossMgr.h"

CFlowMgr_BossElfilis::CFlowMgr_BossElfilis()
    : CLevelFlowMgr(FLOWMGR_BOSSELFILIS)
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

    BOSS->SetActive(false);
}

void CFlowMgr_BossElfilis::tick()
{
    CLevelFlowMgr::tick();
}

void CFlowMgr_BossElfilis::TriggerEvent(UINT _Idx)
{
    if (_Idx == 0)
    {
        SpawnElfilis();
    }
}

void CFlowMgr_BossElfilis::SpawnElfilis()
{
    BOSS->SetActive(true);
}

UINT CFlowMgr_BossElfilis::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CFlowMgr_BossElfilis::LoadFromLevelFile(FILE* _File)
{
    return 0;
}