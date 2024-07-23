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


UINT CKirbyCopyObjScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_ObjectType, sizeof(ObjectCopyType), 1, _File);

    MemoryByte += sizeof(ObjectCopyType);

    return MemoryByte;
}

UINT CKirbyCopyObjScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_ObjectType, sizeof(ObjectCopyType), 1, _File);

    MemoryByte += sizeof(ObjectCopyType);

    return MemoryByte;
}