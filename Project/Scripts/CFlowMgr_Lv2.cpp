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

    SetStartLevel(true);

    LevelStart();

    SetEnterTime(6.8f);
}

void CFlowMgr_Lv2::tick()
{
    CLevelFlowMgr::tick();

    // 시작

    // 끝
}

void CFlowMgr_Lv2::LevelStart()
{
    CLevelFlowMgr::LevelStart();

    // UI (Fade In)
    // Center Mode 설정
    SetFadeEffect(Vec3(255.f, 0.f, 255.f), true, 1.f, 1.25f, true);
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
