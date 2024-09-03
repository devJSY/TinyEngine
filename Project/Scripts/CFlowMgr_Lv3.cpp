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
    SetToneMappingParam(true, false, 0.4f, 0.f);

    SetFadeEffectColor(Vec3(164.f, 44.f, 174.f));

    // BGM
    FadeInBGM(L"sound\\stream\\K15_Park3\\K15_Park3.wav", 0.f, 0.5f, 1.f);
}

void CFlowMgr_Lv3::LevelEnd()
{
    CLevelFlowMgr::LevelEnd();
    FadeOutSound();
    SetFadeEffectColor(Vec3(164.f, 44.f, 174.f));
}

void CFlowMgr_Lv3::LevelRestart()
{
    CLevelFlowMgr::LevelRestart();
    SetFadeEffectColor(Vec3(164.f, 44.f, 174.f));
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