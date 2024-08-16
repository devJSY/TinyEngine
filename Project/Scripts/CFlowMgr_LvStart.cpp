#include "pch.h"
#include "CFlowMgr_LvStart.h"

#include "CUIFlowScript.h"

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
}

void CFlowMgr_LvStart::LevelEnd()
{


    GetFlowScript()->ChangeState(FlowState::End);
}

void CFlowMgr_LvStart::LevelExit()
{
    CLevelFlowMgr::LevelEnd();
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