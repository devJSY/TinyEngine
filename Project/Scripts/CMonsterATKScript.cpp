#include "pch.h"
#include "CMonsterATKScript.h"

#include "CUnitScript.h"

CMonsterATKScript::CMonsterATKScript(UINT _Type)
    : CScript(_Type)
    , m_HitInfo{}
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_HitInfo.Type, "DamageType");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Damage, "Damage");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Damage, "Duration");
}

CMonsterATKScript::CMonsterATKScript(const CMonsterATKScript& _Origin)
    : CScript(_Origin)
    , m_HitInfo(_Origin.m_HitInfo)
{
}

CMonsterATKScript::~CMonsterATKScript()
{
}

void CMonsterATKScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_HitInfo, sizeof(m_HitInfo), 1, _File);
}

void CMonsterATKScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_HitInfo, sizeof(m_HitInfo), 1, _File);
}