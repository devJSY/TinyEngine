#include "pch.h"
#include "CFlowMgr_Lv0.h"

CFlowMgr_Lv0::CFlowMgr_Lv0()
    : CLevelFlowMgr(FLOWMGR_LV0)
{
}

CFlowMgr_Lv0::CFlowMgr_Lv0(const CFlowMgr_Lv0& _Origin)
    : CLevelFlowMgr(_Origin)
{
}

CFlowMgr_Lv0::~CFlowMgr_Lv0()
{
}


void CFlowMgr_Lv0::begin()
{
    CLevelFlowMgr::begin();
}

void CFlowMgr_Lv0::tick()
{
    CLevelFlowMgr::tick();
}

UINT CFlowMgr_Lv0::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_Lv0::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}
