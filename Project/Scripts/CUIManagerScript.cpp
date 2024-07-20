#include "pch.h"
#include "CUIManagerScript.h"
#include <Engine/CLevelMgr.h>

#include "CButtonManagerScript.h"
#include "CUITexManagerScript.h"
#include "CUIAnimManagerScript.h"

CUIManagerScript::CUIManagerScript()
    : CScript(UIMANAGERSCRIPT)
    , m_vUIManagerScript{}
{
}

CUIManagerScript::~CUIManagerScript()
{
    Delete_Map(m_vUIManagerScript);
}

void CUIManagerScript::UIManagerScriptInit()
{
    m_vUIManagerScript.insert(std::make_pair(L"ButtonManagerScript", new CButtonManagerScript));
    m_vUIManagerScript.insert(std::make_pair(L"TexManagerScript", new CUITexManagerScript));
    m_vUIManagerScript.insert(std::make_pair(L"AnimManagerScript", new CUIAnimManagerScript));

    map<wstring, CScript*>::iterator iter = m_vUIManagerScript.begin();

    for (; iter != m_vUIManagerScript.end(); iter++)
    {
        (*iter).second->begin();
    }
}

UINT CUIManagerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CUIManagerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

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