#include "pch.h"
#include "CLensFlareScript.h"
#include <Engine\\CRenderMgr.h>

CLensFlareScript::CLensFlareScript()
    : CScript(LENSFLARESCRIPT)
    , m_bDepthCheck(true)
    , m_Spacing(5.f)
    , m_CenterOffset()
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bDepthCheck, "Depth Check");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Spacing, "Spacing", 1e-2f);
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_CenterOffset, "Center Offset", 1e-2f);
}

CLensFlareScript::CLensFlareScript(const CLensFlareScript& origin)
    : CScript(origin)
    , m_bDepthCheck(origin.m_bDepthCheck)
    , m_Spacing(origin.m_Spacing)
    , m_CenterOffset(origin.m_CenterOffset)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bDepthCheck, "Depth Check");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Spacing, "Spacing", 1e-2f);
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_CenterOffset, "Center Offset", 1e-2f);
}

CLensFlareScript::~CLensFlareScript()
{
}

void CLensFlareScript::begin()
{
}

void CLensFlareScript::tick()
{
    if (nullptr != GetOwner()->MeshRender() && nullptr != MeshRender()->GetMaterial(0))
    {
        MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, m_bDepthCheck);
        MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, m_Spacing);
        MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_0, GetSunNDCPos());
        MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_1, m_CenterOffset);
    }
}

Vec4 CLensFlareScript::GetSunNDCPos()
{
    // MainCam
    CCamera* _pCam = CRenderMgr::GetInst()->GetMainCamera();

    if (nullptr == _pCam)
    {
        return Vec4(-100, -100, -100, -100);
    }

    Matrix VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();

    Vec3 SunWorldPos = Transform()->GetWorldPos();

    // WolrdPos -> NDC
    Vec4 SunNDCPos = Vector4::Transform(Vec4(SunWorldPos.x, SunWorldPos.y, SunWorldPos.z, 1.f), VPMatrix);

    // Perspective Division
    SunNDCPos.x /= SunNDCPos.w;
    SunNDCPos.y /= SunNDCPos.w;
    SunNDCPos.z /= SunNDCPos.w;

    return SunNDCPos;
}

UINT CLensFlareScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bDepthCheck, sizeof(bool), 1, _File);
    fwrite(&m_Spacing, sizeof(float), 1, _File);
    fwrite(&m_CenterOffset, sizeof(Vec2), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec2);
    return MemoryByte;
}

UINT CLensFlareScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bDepthCheck, sizeof(bool), 1, _File);
    fread(&m_Spacing, sizeof(float), 1, _File);
    fread(&m_CenterOffset, sizeof(Vec2), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(Vec2);
    return MemoryByte;
}
