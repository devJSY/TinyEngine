#include "pch.h"
#include "CUIDashEffectScript.h"

CUIDashEffectScript::CUIDashEffectScript()
    : CScript(UIDASHEFFECTSCRIPT)
    , m_fRotateSpeed(0.f)
    , m_fRotateYSpeed(0.f)
    , m_fTextureValue(0.f)
    , m_fTextureValueSpeed(0.f)
    , m_fMaxThreshHold(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRotateYSpeed, "RotateY Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTextureValueSpeed, "TextureValue Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxThreshHold, "Max ThreshHold");
}

CUIDashEffectScript::CUIDashEffectScript(const CUIDashEffectScript& Origin)
    : CScript(Origin)
    , m_fRotateSpeed(0.f)
    , m_fRotateYSpeed(Origin.m_fRotateYSpeed)
    , m_fTextureValue(0.f)
    , m_fTextureValueSpeed(Origin.m_fTextureValueSpeed)
    , m_fMaxThreshHold(Origin.m_fMaxThreshHold)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fRotateYSpeed, "RotateY Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTextureValueSpeed, "TextureValue Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMaxThreshHold, "Max ThreshHold");
}

CUIDashEffectScript::~CUIDashEffectScript()
{
}

void CUIDashEffectScript::tick()
{
    CMeshRender* pRender = MeshRender();
    CTransform* pTr = Transform();

    if (nullptr != pRender && nullptr != pTr)
    {
        m_fTextureValue += DT * m_fTextureValueSpeed;
        pRender->GetMaterial(0)->SetScalarParam(FLOAT_0, m_fTextureValue);

        Vec3 vRot = pTr->GetLocalRotation();
        vRot.z += DT * m_fRotateYSpeed;
        pTr->SetLocalRotation(vRot);
    }
}

UINT CUIDashEffectScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fRotateYSpeed, sizeof(float), 1, _File);
    fwrite(&m_fTextureValueSpeed, sizeof(float), 1, _File);
    fwrite(&m_fMaxThreshHold, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CUIDashEffectScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fRotateYSpeed, sizeof(float), 1, _File);
    fread(&m_fTextureValueSpeed, sizeof(float), 1, _File);
    fread(&m_fMaxThreshHold, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}