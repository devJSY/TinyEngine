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

void CUIUnitManagerScript::begin()
{
}

void CUIUnitManagerScript::tick()
{
    vector<UnitTask>::iterator iter = m_vUnitTasks.begin();

    for (; iter != m_vUnitTasks.end();iter++)
    {

    }



}