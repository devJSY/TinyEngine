#include "pch.h"
#include "CShockWaveScript.h"
#include <Engine\CRenderMgr.h>

CShockWaveScript::CShockWaveScript()
    : CScript(SHOCKWAVESCRIPT)
    , m_CircleSize(0.f)
    , m_CircleSizeSpeed(1.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CircleSizeSpeed, "Circle Size Speed");
}

CShockWaveScript::CShockWaveScript(const CShockWaveScript& origin)
    : CScript(origin)
    , m_CircleSize(0.f)
    , m_CircleSizeSpeed(origin.m_CircleSizeSpeed)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CircleSizeSpeed, "Circle Size Speed");
}

CShockWaveScript::~CShockWaveScript()
{
}

void CShockWaveScript::tick()
{
    // billboard
    if (nullptr != CAMERACTRL)
    {
        Vec3 LookDir = (CAMERACTRL->GetOwner()->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
        Transform()->SetDirection(LookDir);
    }

    m_CircleSize += m_CircleSizeSpeed * DT;

    // นÝบน
    if (m_CircleSize > 0.3f)
    {
        m_CircleSize = 0.f;
    }

    if (nullptr != GetOwner()->GetRenderComponent())
    {
        Ptr<CMaterial> pMatrl = GetOwner()->GetRenderComponent()->GetMaterial(0);
        pMatrl->SetScalarParam(FLOAT_0, m_CircleSize);
    }
}

UINT CShockWaveScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_CircleSizeSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    return MemoryByte;
}

UINT CShockWaveScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_CircleSizeSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    return MemoryByte;
}
