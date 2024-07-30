#include "pch.h"
#include "CMonsterATKScript.h"

CMonsterATKScript::CMonsterATKScript(UINT _Type)
    : CScript(_Type)
{
}

CMonsterATKScript::CMonsterATKScript(const CMonsterATKScript& _Origin)
    : CScript(_Origin)
{
}

CMonsterATKScript::~CMonsterATKScript()
{
}

UINT CMonsterATKScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CMonsterATKScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}