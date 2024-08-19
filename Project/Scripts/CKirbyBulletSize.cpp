#include "pch.h"
#include "CKirbyBulletSize.h"

CKirbyBulletSize::CKirbyBulletSize()
    : CScript(KIRBYBULLETSIZE)
    , m_SizeType(1)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_SizeType, "Size (1~3)");
}

CKirbyBulletSize::CKirbyBulletSize(const CKirbyBulletSize& _Origin)
    : CScript(_Origin)
    , m_SizeType(_Origin.m_SizeType)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_SizeType, "Size (1~3)");
}

CKirbyBulletSize::~CKirbyBulletSize()
{
}

UINT CKirbyBulletSize::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_SizeType, sizeof(int), 1, _File);
    MemoryByte += sizeof(int);

    return MemoryByte;
}

UINT CKirbyBulletSize::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_SizeType, sizeof(int), 1, _File);
    MemoryByte += sizeof(int);

    return MemoryByte;
}