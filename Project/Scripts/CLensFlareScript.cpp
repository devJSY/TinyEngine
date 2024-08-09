#include "pch.h"
#include "CLensFlareScript.h"
#include <Engine\\CRenderMgr.h>

CLensFlareScript::CLensFlareScript()
    : CScript(LENSFLARESCRIPT)
    , m_SunWorldPos()
    , m_Spacing(1.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_SunWorldPos, "Sun World Position");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Spacing, "Spacing", 1e-3f);
}

CLensFlareScript::CLensFlareScript(const CLensFlareScript& origin)
    : CScript(origin)
    , m_SunWorldPos(origin.m_SunWorldPos)
    , m_Spacing(origin.m_Spacing)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_SunWorldPos, "Sun World Position");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Spacing, "Spacing", 1e-3f);
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
        MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_0, GetSunNDCPos());
        MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, m_Spacing);
    }
}

Vec2 CLensFlareScript::GetSunNDCPos()
{
    // MainCam
    CCamera* _pCam = CRenderMgr::GetInst()->GetMainCamera();

    if (nullptr == _pCam)
    {
        return Vec2(-100, -100);
    }

    Matrix VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();

    // WolrdPos -> NDC
    Vec4 SunNDCPos = Vector4::Transform(Vec4(m_SunWorldPos.x, m_SunWorldPos.y, m_SunWorldPos.z, 1.f), VPMatrix);

    // Perspective Division
    SunNDCPos.x /= SunNDCPos.w;
    SunNDCPos.y /= SunNDCPos.w;
    SunNDCPos.z /= SunNDCPos.w;

    return Vec2(SunNDCPos.x, SunNDCPos.y);
}

UINT CLensFlareScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_SunWorldPos, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(Vec3);
    return MemoryByte;
}

UINT CLensFlareScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_SunWorldPos, sizeof(Vec3), 1, _File);

    MemoryByte += sizeof(Vec3);
    return MemoryByte;
}
