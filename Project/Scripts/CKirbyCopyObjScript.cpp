#include "pch.h"
#include "CKirbyCopyObjScript.h"

CKirbyCopyObjScript::CKirbyCopyObjScript()
    : CScript(KIRBYCOPYOBJSCRIPT)
    , m_ObjectType(ObjectCopyType::NONE)
{
    //@TODO enum으로 script param 설정하기
    AddScriptParam(SCRIPT_PARAM::INT, &m_ObjectType, "ObjectType UINT");
}

CKirbyCopyObjScript::CKirbyCopyObjScript(const CKirbyCopyObjScript& _Origin)
    : CScript(KIRBYCOPYOBJSCRIPT)
    , m_ObjectType(_Origin.m_ObjectType)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_ObjectType, "ObjectType UINT");
}

CKirbyCopyObjScript::~CKirbyCopyObjScript()
{
}


void CKirbyCopyObjScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_ObjectType, sizeof(ObjectCopyType), 1, _File);
}

void CKirbyCopyObjScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_ObjectType, sizeof(ObjectCopyType), 1, _File);
}