#include "pch.h"
#include "CScript.h"

CScript::CScript(SCRIPT_TYPE _type)
    : CComponent(COMPONENT_TYPE::SCRIPT)
    , m_iScriptType(_type)
{
}

CScript::~CScript()
{
}
