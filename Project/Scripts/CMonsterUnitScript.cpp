#include "pch.h"
#include "CMonsterUnitScript.h"

CMonsterUnitScript::CMonsterUnitScript()
    : CUnitScript(MONSTERUNITSCRIPT)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP current");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
}

CMonsterUnitScript::CMonsterUnitScript(const CMonsterUnitScript& _Origin)
    : CUnitScript(MONSTERUNITSCRIPT)
{
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
