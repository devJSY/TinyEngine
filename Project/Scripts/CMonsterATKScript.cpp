#include "pch.h"
#include "CMonsterATKScript.h"

CMonsterATKScript::CMonsterATKScript(UINT _Type)
    : CScript(_Type)
    , m_HitInfo{}
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_HitInfo.Type, "DamageType");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Damage, "Damage");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Duration, "Duration");
}

CMonsterATKScript::CMonsterATKScript(const CMonsterATKScript& _Origin)
    : CScript(_Origin)
    , m_HitInfo(_Origin.m_HitInfo)
{
}

CMonsterATKScript::~CMonsterATKScript()
{
}

UINT CMonsterATKScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_HitInfo, sizeof(UnitHit), 1, _File);

    MemoryByte += sizeof(UnitHit);

    return MemoryByte;
}

UINT CMonsterATKScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_HitInfo, sizeof(UnitHit), 1, _File);

    MemoryByte += sizeof(UnitHit);

    return MemoryByte;
}