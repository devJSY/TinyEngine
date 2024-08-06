#include "pch.h"
#include "CUIAnimManagerScript.h"

#include "CUIFlowScript.h"

#include "CUIStartSceneFlowScript.h"

CUIAnimManagerScript::CUIAnimManagerScript()
    : CScript(UIANIMMANAGERSCRIPT)
    , m_vFlowScript{}
{
}

CUIAnimManagerScript::~CUIAnimManagerScript()
{
    Delete_Vec(m_vFlowScript);
}

void CUIAnimManagerScript::begin()
{
    m_vFlowScript.resize(static_cast<size_t>(SceneType::End));

    // TODO : Scene Flow ÃÊ±âÈ­
    {
        CUIStartSceneFlowScript* script = new CUIStartSceneFlowScript;
        script->SetLoop(true);
        script->SetWaitTime(2.f);

        script->begin();
        m_vFlowScript[(UINT)SceneType::StartScene] = script;
    }

}

void CUIAnimManagerScript::tick()
{
    nullptr != m_vFlowScript[(UINT)m_eType] ? m_vFlowScript[(UINT)m_eType]->tick() : void();
}

UINT CUIAnimManagerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CUIAnimManagerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}