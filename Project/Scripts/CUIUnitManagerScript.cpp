#include "pch.h"
#include "CUIUnitManagerScript.h"

CUIUnitManagerScript::CUIUnitManagerScript()
    : CScript(UIUNITMANAGERSCRIPT)
    , m_vUnitTasks{}
{
}

CUIUnitManagerScript::~CUIUnitManagerScript()
{
}

UINT CUIUnitManagerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CUIUnitManagerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

void CUIUnitManagerScript::begin()
{
}

void CUIUnitManagerScript::tick()
{
    vector<UnitTask>::iterator iter = m_vUnitTasks.begin();

    for (; iter != m_vUnitTasks.end(); iter++)
    {
    }
}