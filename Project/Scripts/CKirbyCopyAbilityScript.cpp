#include "pch.h"
#include "CKirbyCopyAbilityScript.h"

CKirbyCopyAbilityScript::CKirbyCopyAbilityScript()
    : CScript(KIRBYCOPYABILITYSCRIPT)
    , m_AbilityType((UINT)AbilityCopyType::NONE)
{
    //@TODO Enum으로 Script param 추가하기
    AddScriptParam(SCRIPT_PARAM::INT, &m_AbilityType, "AbilityType UINT");

}

CKirbyCopyAbilityScript::CKirbyCopyAbilityScript(const CKirbyCopyAbilityScript& _Origin)
    : CScript(KIRBYCOPYABILITYSCRIPT)
    , m_AbilityType(_Origin.m_AbilityType)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_AbilityType, "AbilityType UINT");
}

CKirbyCopyAbilityScript::~CKirbyCopyAbilityScript()
{
}


UINT CKirbyCopyAbilityScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_AbilityType, sizeof(UINT), 1, _File);
    
    MemoryByte += sizeof(UINT);

    return MemoryByte;
}

UINT CKirbyCopyAbilityScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_AbilityType, sizeof(UINT), 1, _File);

    MemoryByte += sizeof(UINT);

    return MemoryByte;
}