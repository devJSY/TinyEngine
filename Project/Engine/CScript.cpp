#include "pch.h"
#include "CScript.h"

CScript::CScript(UINT _ScriptType)
    : CComponent(COMPONENT_TYPE::SCRIPT)
    , m_ScriptType(_ScriptType)
{
}

CScript::~CScript()
{
}
