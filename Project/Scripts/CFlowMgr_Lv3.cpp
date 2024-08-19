#include "pch.h"
#include "CFlowMgr_Lv3.h"

CFlowMgr_Lv3::CFlowMgr_Lv3()
    : CLevelFlowMgr(FLOWMGR_LV3)
{
}

CFlowMgr_Lv3::CFlowMgr_Lv3(const CFlowMgr_Lv3& _Origin)
    : CLevelFlowMgr(_Origin)
{
}

CFlowMgr_Lv3::~CFlowMgr_Lv3()
{
}

void CFlowMgr_Lv3::begin()
{
    CLevelFlowMgr::begin();

    SetStartLevel(false);

    LevelStart();
    SetToneMappingParam(true, true, 0.6f, 0.23f);
}

void CFlowMgr_Lv3::tick()
{
    CLevelFlowMgr::tick();
}

UINT CFlowMgr_Lv3::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_Lv3::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}
