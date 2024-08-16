#include "pch.h"
#include "CFlowMgr_Lv2.h"

CFlowMgr_Lv2::CFlowMgr_Lv2()
    : CLevelFlowMgr(FLOWMGR_LV2)
{
}

CFlowMgr_Lv2::CFlowMgr_Lv2(const CFlowMgr_Lv2& _Origin)
    : CLevelFlowMgr(_Origin)
{
}

CFlowMgr_Lv2::~CFlowMgr_Lv2()
{
}


void CFlowMgr_Lv2::begin()
{
    CLevelFlowMgr::begin();
}

void CFlowMgr_Lv2::tick()
{
    CLevelFlowMgr::tick();

    // Ω√¿€

    // ≥°
}

UINT CFlowMgr_Lv2::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_Lv2::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}
