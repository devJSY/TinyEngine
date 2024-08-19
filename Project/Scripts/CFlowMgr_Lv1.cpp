#include "pch.h"
#include "CFlowMgr_Lv1.h"

CFlowMgr_Lv1::CFlowMgr_Lv1()
    : CLevelFlowMgr(FLOWMGR_LV1)
{
}

CFlowMgr_Lv1::CFlowMgr_Lv1(const CFlowMgr_Lv1& _Origin)
    : CLevelFlowMgr(_Origin)
{
}

CFlowMgr_Lv1::~CFlowMgr_Lv1()
{
}

void CFlowMgr_Lv1::begin()
{
    CLevelFlowMgr::begin();

    SetStartLevel(true);

    LevelStart();

    SetEnterTime(6.5f);

    SetToneMappingParam(true, true, 1.2f, 0.8f);
}

void CFlowMgr_Lv1::tick()
{
    CLevelFlowMgr::tick();
}

UINT CFlowMgr_Lv1::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_Lv1::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}
