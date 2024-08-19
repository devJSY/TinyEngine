#include "pch.h"
#include "CUIFlowMgr.h"

#include "CLevelFlowMgr.h"

#include "CUIStartSceneFlowScript.h"

CUIFlowMgr::CUIFlowMgr()
    : CScript(UIFLOWMGR)
{
}

CUIFlowMgr::~CUIFlowMgr()
{
    delete m_pFlowScript;
}

void CUIFlowMgr::begin()
{
    switch (m_eType)
    {
    case SceneType::StartScene: {
        CUIStartSceneFlowScript* pScript = new CUIStartSceneFlowScript;
        pScript->SetLoop(true);
        pScript->SetWaitTime(2.f);
        pScript->SetUIAnimManager(m_pAnimMgrScript);

        pScript->begin();
        m_pFlowScript = pScript;
    }
    break;
    case SceneType::RobbySccene:
        break;
    case SceneType::PlayScene:
        break;
    case SceneType::End:
        break;
    default:
        break;
    }
}

void CUIFlowMgr::tick()
{
    if (nullptr != m_pFlowScript)
    {
        m_pFlowScript->tick();
    }
}

UINT CUIFlowMgr::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CUIFlowMgr::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}