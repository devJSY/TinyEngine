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

    SetEnterTime(6.8f);

    SetToneMappingParam(true, true, 1.2f, 0.8f);

    SetFadeEffectColor(Vec3(115.f, 156.f, 28.f));
}

void CFlowMgr_Lv1::tick()
{
    CLevelFlowMgr::tick();
}

void CFlowMgr_Lv1::LevelStart()
{
    CLevelFlowMgr::LevelStart();
    GamePlayStatic::PlayBGM(L"sound\\stream\\K15_Grassland1\\K15_Grassland1.marker.wav", 0.5f);
}

void CFlowMgr_Lv1::LevelEnd()
{
    CLevelFlowMgr::LevelEnd();

    if (L"Level1-1-2.tLevel" == GetCurrentLevel())
    {
        SetFadeEffectColor(Vec3(255.f, 150.f, 100.f));
    }
    else
    {
        SetFadeEffectColor(Vec3(115.f, 156.f, 28.f));
    }
}

void CFlowMgr_Lv1::LevelRestart()
{
    CLevelFlowMgr::LevelRestart();
    if (L"Level1-1-2.tLevel" == GetCurrentLevel())
    {
        SetFadeEffectColor(Vec3(115.f, 156.f, 28.f));
    }
    else
    {
        SetFadeEffectColor(Vec3(115.f, 156.f, 28.f));
    }
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