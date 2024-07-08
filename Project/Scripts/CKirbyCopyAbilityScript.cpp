#include "pch.h"
#include "CKirbyCopyAbilityScript.h"

CKirbyCopyAbilityScript::CKirbyCopyAbilityScript()
    : CScript(KIRBYCOOPYABILITYSCRIPT)
    , m_AbilityType(AbilityCopyType::NONE)
{
    //@TODO Enum으로 Script param 추가하기
}

CKirbyCopyAbilityScript::CKirbyCopyAbilityScript(const CKirbyCopyAbilityScript& _Origin)
    : CScript(KIRBYCOOPYABILITYSCRIPT)
    , m_AbilityType(_Origin.m_AbilityType)
{
}

CKirbyCopyAbilityScript::~CKirbyCopyAbilityScript()
{
}

void CKirbyCopyAbilityScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_AbilityType, sizeof(AbilityCopyType), 1, _File);
}

void CKirbyCopyAbilityScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_AbilityType, sizeof(AbilityCopyType), 1, _File);
}