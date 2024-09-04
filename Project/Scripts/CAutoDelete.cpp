#include "pch.h"
#include "CAutoDelete.h"

CAutoDelete::CAutoDelete()
    : CScript(AUTODELETE)
    , m_Duration(1.f)
    , m_Acc(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
}

CAutoDelete::CAutoDelete(const CAutoDelete& _Origin)
    : CScript(AUTODELETE)
    , m_Duration(_Origin.m_Duration)
    , m_Acc(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
}

CAutoDelete::~CAutoDelete()
{
}


void CAutoDelete::begin()
{
    m_Acc = 0.f;
}

void CAutoDelete::tick()
{
    m_Acc += DT;

    if (m_Acc > m_Duration)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

UINT CAutoDelete::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_Duration, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CAutoDelete::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_Duration, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}

