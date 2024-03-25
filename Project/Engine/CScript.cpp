#include "pch.h"
#include "CScript.h"

CScript::CScript(UINT _ScriptType)
    : CComponent(COMPONENT_TYPE::SCRIPT)
    , m_ScriptType(_ScriptType)
    , m_vecParam{}
{
}

CScript::CScript(const CScript& origin)
    : CComponent(origin)
    , m_ScriptType(origin.m_ScriptType)
    , m_vecParam{}
{
}

CScript::~CScript()
{
}
