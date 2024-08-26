#include "pch.h"
#include "CFlowMgr_LvStart.h"

#include "CUIFlowScript.h"

bool CFlowMgr_LvStart::m_bEnter = true;

CFlowMgr_LvStart::CFlowMgr_LvStart()
    : CLevelFlowMgr(FLOWMGR_LVSTART)
{
}

CFlowMgr_LvStart::CFlowMgr_LvStart(const CFlowMgr_LvStart& _Origin)
    : CLevelFlowMgr(_Origin)
{
}

CFlowMgr_LvStart::~CFlowMgr_LvStart()
{
}

void CFlowMgr_LvStart::begin()
{
    CLevelFlowMgr::begin();

    SetUILevel(true);

    SetStartLevel(false);

    LevelStart();

    OffDimensionFade();
    SetToneMappingParam(false);
    SetUIDOFEffect();

    // SetLoadingUIColor(Vec3(255.f, 100.f, 129.f));
}

void CFlowMgr_LvStart::LevelStart()
{
    ResetFadeEffectTimer();
    if (m_bEnter)
    {
        m_bEnter = false;
        ActiveFadeEffect(false);
    }
    else
    {
        SetFadeEffect(Vec3(252.f, 75.f, 129.f), true, 1.f, 1.25f, true);
    }

    Ptr<CPrefab> MainPlayerFreb = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Main Player.pref");

    if (MainPlayerFreb.Get())
    {
        GamePlayStatic::DeleteAsset(ASSET_TYPE::PREFAB, MainPlayerFreb.Get());
    }
}

void CFlowMgr_LvStart::LevelEnd()
{
    CLevelFlowMgr::LevelEnd();
    ActiveFadeEffect(true);
    SetFadeEffect(Vec3(252.f, 75.f, 129.f), false, 1.f, 1.25f, true);
}

void CFlowMgr_LvStart::LevelRestart()
{
    CLevelFlowMgr::LevelRestart();
    ActiveFadeEffect(true);
    SetFadeEffect(Vec3(252.f, 75.f, 129.f), false, 1.f, 1.25f, true);
}

void CFlowMgr_LvStart::RobbyLevel()
{
    return;
}

UINT CFlowMgr_LvStart::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_LvStart::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}