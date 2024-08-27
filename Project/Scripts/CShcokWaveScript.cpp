#include "pch.h"
#include "CShcokWaveScript.h"
#include <Engine\CRenderMgr.h>

CShcokWaveScript::CShcokWaveScript()
    : CScript(SHCOKWAVESCRIPT)
    , m_CircleSize(0.f)
    , m_CircleSizeSpeed(1.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CircleSizeSpeed, "Circle Size Speed");
}

CShcokWaveScript::CShcokWaveScript(const CShcokWaveScript& origin)
    : CScript(origin)
    , m_CircleSize(0.f)
    , m_CircleSizeSpeed(origin.m_CircleSizeSpeed)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CircleSizeSpeed, "Circle Size Speed");
}

CShcokWaveScript::~CShcokWaveScript()
{
}

void CShcokWaveScript::tick()
{
    CCamera* MainCam = CRenderMgr::GetInst()->GetMainCamera();

    // billboard
    if (nullptr != MainCam)
    {
        Vec3 LookDir = MainCam->Transform()->GetWorldPos() - Transform()->GetWorldPos();
        LookDir.y = 0.f;
        LookDir.Normalize();
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

UINT CShcokWaveScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_CircleSizeSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    return MemoryByte;
}

UINT CShcokWaveScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_CircleSizeSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    return MemoryByte;
}
