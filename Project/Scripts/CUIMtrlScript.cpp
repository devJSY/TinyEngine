#include "pch.h"
#include "CUIMtrlScript.h"

CUIMtrlScript::CUIMtrlScript()
    : CScript(UIMTRLSCRIPT)
    , m_vPatternUV1{}
    , m_vPatternUV2{}
    , m_fSpeed1(0.f)
    , m_fSpeed2(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed1, "UV Speed1");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed2, "UV Speed2");
}

CUIMtrlScript::~CUIMtrlScript()
{
}

void CUIMtrlScript::tick()
{
    if (MeshRender())
    {
        if (nullptr != MeshRender()->GetMaterial(0))
        {
            MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_0, Vec2(m_vPatternUV1.x -= DT * m_fSpeed1,m_vPatternUV1.y += DT * m_fSpeed1));
            MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_1, Vec2(m_vPatternUV2.x -= DT * m_fSpeed2, m_vPatternUV2.y += DT * m_fSpeed2));
        }
    }
}

UINT CUIMtrlScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fSpeed1, sizeof(float), 1, _File);
    fwrite(&m_fSpeed1, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIMtrlScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fSpeed1, sizeof(float), 1, _File);
    fread(&m_fSpeed1, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}