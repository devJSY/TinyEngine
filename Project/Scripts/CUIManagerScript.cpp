#include "pch.h"
#include "CUIManagerScript.h"
#include <Engine/CLevelMgr.h>

#include "CButtonManagerScript.h"

CUIManagerScript::CUIManagerScript()
    : CScript(UIMANAGERSCRIPT)
{
}

CUIManagerScript::~CUIManagerScript()
{
    Delete_Vec(m_vUIManagerScript);
}

void CUIManagerScript::UIManagerScriptInit()
{
    m_vUIManagerScript.push_back(new CButtonManagerScript);


    for (size_t i = 0; i < m_vUIManagerScript.size();i++)
    {
        m_vUIManagerScript[i]->begin();
    }
}

void CUIManagerScript::begin()
{
    UIManagerScriptInit();
}

void CUIManagerScript::tick()
{ 
    for (size_t i = 0; i < m_vUIManagerScript.size(); i++)
    {
        m_vUIManagerScript[i]->tick();
    }
}