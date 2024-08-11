#include "pch.h"
#include "CFlowMgr_Lv4.h"

CFlowMgr_Lv4::CFlowMgr_Lv4()
    : CLevelFlowMgr(FLOWMGR_LV4)
{
}

CFlowMgr_Lv4::CFlowMgr_Lv4(const CFlowMgr_Lv4& _Origin)
    : CLevelFlowMgr(_Origin)
{
}

CFlowMgr_Lv4::~CFlowMgr_Lv4()
{
}

void CFlowMgr_Lv4::begin()
{
    CLevelFlowMgr::begin();
}

void CFlowMgr_Lv4::tick()
{
    CLevelFlowMgr::tick();
}

UINT CFlowMgr_Lv4::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_Lv4::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}

