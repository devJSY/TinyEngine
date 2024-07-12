#include "pch.h"
#include "CKirbyCopyAbilityScript.h"

CKirbyCopyAbilityScript::CKirbyCopyAbilityScript()
    : CScript(KIRBYCOPYABILITYSCRIPT)
    , m_AbilityType((UINT)AbilityCopyType::NONE)
{
    //@TODO Enum���� Script param �߰��ϱ�
    AddScriptParam(SCRIPT_PARAM::INT, &m_AbilityType, "AbilityType UINT");

}

CKirbyCopyAbilityScript::CKirbyCopyAbilityScript(const CKirbyCopyAbilityScript& _Origin)
    : CScript(KIRBYCOPYABILITYSCRIPT)
    , m_AbilityType(_Origin.m_AbilityType)
{
}

CKirbyCopyAbilityScript::~CKirbyCopyAbilityScript()
{
}


void CKirbyCopyAbilityScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_AbilityType, sizeof(UINT), 1, _File);
}

void CKirbyCopyAbilityScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_AbilityType, sizeof(UINT), 1, _File);
}