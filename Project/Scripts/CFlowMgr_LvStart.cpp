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
        SetFadeEffect(Vec3(255.f, 0.f, 255.f), true, 1.f, 1.25f, true);
    }
}

void CFlowMgr_LvStart::LevelEnd()
{
    CLevelFlowMgr::LevelEnd();

    ActiveFadeEffect(true);
    SetFadeEffect(Vec3(255.f, 0.f, 255.f), false, 1.f, 1.25f, true);
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