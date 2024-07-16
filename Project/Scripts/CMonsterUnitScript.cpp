#include "pch.h"
#include "CMonsterUnitScript.h"

CMonsterUnitScript::CMonsterUnitScript(UINT _Type)
    : CUnitScript(_Type)
    , m_pTargetObj(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP current");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
}

CMonsterUnitScript::CMonsterUnitScript(const CMonsterUnitScript& _Origin)
    : CUnitScript(_Origin)
    , m_pTargetObj(_Origin.m_pTargetObj)
{
    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP current");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
}

CMonsterUnitScript::~CMonsterUnitScript()
{
}

void CMonsterUnitScript::SaveToLevelFile(FILE* _File)
{
    CUnitScript::SaveToLevelFile(_File);
}

void CMonsterUnitScript::LoadFromLevelFile(FILE* _File)
{
    CUnitScript::LoadFromLevelFile(_File);
}
