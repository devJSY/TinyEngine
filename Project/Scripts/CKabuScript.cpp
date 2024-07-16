#include "pch.h"
#include "CKabuScript.h"

CKabuScript::CKabuScript()
    : CMonsterUnitScript(KABUSCRIPT)
{
}

CKabuScript::~CKabuScript()
{
}

void CKabuScript::begin()
{
}

void CKabuScript::tick()
{
}

void CKabuScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CKabuScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}