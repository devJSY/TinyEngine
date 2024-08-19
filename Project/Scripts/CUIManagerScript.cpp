#include "pch.h"
#include "CUIManagerScript.h"
#include <Engine/CLevelMgr.h>

#include "CButtonManagerScript.h"
#include "CUITexManagerScript.h"
#include "CUIAnimManagerScript.h"
#include "CUIFlowMgr.h"

CUIManagerScript::CUIManagerScript()
    : CScript(UIMANAGERSCRIPT)
    , m_vUIManagerScript{}
    , m_eSceneState(SceneType::StartScene)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_eSceneState, "SceneState");
}

CUIManagerScript::~CUIManagerScript()
{
    Delete_Map(m_vUIManagerScript);
}

void CUIManagerScript::UIManagerScriptInit()
{
    CUIFlowMgr* manager = new CUIFlowMgr;
    manager->SetSceneType(m_eSceneState);

    CUIAnimManagerScript* pAnimManager = new CUIAnimManagerScript;

    m_vUIManagerScript.insert(std::make_pair(L"ButtonManagerScript", new CButtonManagerScript));
    m_vUIManagerScript.insert(std::make_pair(L"TexManagerScript", new CUITexManagerScript));
    m_vUIManagerScript.insert(std::make_pair(L"AnimManagerScript", pAnimManager));
    m_vUIManagerScript.insert(std::make_pair(L"FlowManagerScript", manager));

    manager->SetAnimMgrScript(pAnimManager);

    map<wstring, CScript*>::iterator iter = m_vUIManagerScript.begin();

    for (; iter != m_vUIManagerScript.end(); iter++)
    {
        (*iter).second->begin();
    }


}

UINT CUIManagerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    /* UINT SceneType = (UINT)m_eSceneState;

     fwrite(&SceneType, sizeof(UINT), 1, _File);

     MemoryByte += sizeof(UINT);*/

    return MemoryByte;
}

UINT CUIManagerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    /*   UINT SceneType1 = 0;

       fread(&SceneType1, sizeof(UINT), 1, _File);

       m_eSceneState = SceneType(SceneType1);

       MemoryByte += SceneType1;*/

    return MemoryByte;
}

void CUIManagerScript::begin()
{
    UIManagerScriptInit();
}

void CUIManagerScript::tick()
{
    map<wstring, CScript*>::iterator iter = m_vUIManagerScript.begin();

    for (; iter != m_vUIManagerScript.end(); iter++)
    {
        (*iter).second->tick();
    }
}