#include "pch.h"
#include "CUIAnimScript.h"

CUIAnimScript::CUIAnimScript()
    : CScript(UIANIMSCRIPT)
    , m_eState(UIAnimState::PrePared)
    , m_bIsFinsih(false)
    , m_fWaitTime(0.f)
    , m_fAccTime(0.f)
    , m_bLoopUI(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fWaitTime, "WaitTime");
}

CUIAnimScript::CUIAnimScript(UINT _ScriptType)
    : CScript(_ScriptType)
    , m_eState(UIAnimState::PrePared)
    , m_bIsFinsih(false)
    , m_fWaitTime(0.f)
    , m_fAccTime(0.f)
    , m_bLoopUI(false)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fWaitTime, "WaitTime");
}

CUIAnimScript::CUIAnimScript(const CUIAnimScript& Origin)
    : CScript(Origin)
    , m_eState(UIAnimState::PrePared)
    , m_bIsFinsih(false)
    , m_fWaitTime(Origin.m_fWaitTime)
    , m_fAccTime(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fWaitTime, "WaitTime");
}

CUIAnimScript::~CUIAnimScript()  
{
}

void CUIAnimScript::begin()
{
}

void CUIAnimScript::tick()
{
    if (UIAnimState::PrePared == m_eState)
    {
        m_fAccTime += DT;
        if (m_fAccTime >= m_fWaitTime)
        {
            m_eState = UIAnimState::Tick;
            m_fAccTime = 0.f;
        }
    }
}

void CUIAnimScript::CommonUIEnter()
{
}

void CUIAnimScript::CommonUIExit()
{
    //SetFinish(true);
}

UINT CUIAnimScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fWaitTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIAnimScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fWaitTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}
