#include "pch.h"
#include "CFadeOutScript.h"
#include "define.h"
#include <Engine\\CRenderMgr.h>
#include "CPlayerMgr.h"

CFadeOutScript::CFadeOutScript()
    : CScript(FADEOUTSCRIPT)
    , pTarget(nullptr)
    , m_Duration(1.f)
    , m_ElapsedTime(0.f)
    , m_RotateSpeed(1.25f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotateSpeed, "Rotate Speed");
}

CFadeOutScript::CFadeOutScript(const CFadeOutScript& origin)
    : CScript(origin)
    , pTarget(nullptr)
    , m_Duration(origin.m_Duration)
    , m_ElapsedTime(0.f)
    , m_RotateSpeed(origin.m_RotateSpeed)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, "Duration");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotateSpeed, "Rotate Speed");
}

CFadeOutScript::~CFadeOutScript()
{
}
void CFadeOutScript::begin()
{
    pTarget = PLAYER;
    m_ElapsedTime = 0.f;
}

void CFadeOutScript::tick()
{
    if (nullptr == pTarget)
    {
        pTarget = PLAYER;
        return;
    }

    m_ElapsedTime += DT;
    if (m_ElapsedTime > m_Duration)
    {
        m_ElapsedTime = m_Duration;
    }

    if (nullptr != GetOwner()->MeshRender() && nullptr != MeshRender()->GetMaterial(0))
    {
        MeshRender()->GetMaterial(0)->SetScalarParam(VEC2_0, GetTargetNDCPos());
        MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, m_ElapsedTime / m_Duration);
        MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, XM_PI * m_ElapsedTime * m_RotateSpeed);
    }
}

Vec2 CFadeOutScript::GetTargetNDCPos()
{
    if (nullptr == pTarget)
    {
        // NDC 영역이 아닌 임의의 위치 설정
        return Vec2(-100, -100);
    }

    Vec3 TargetPos = pTarget->Transform()->GetWorldPos();

    // MainCam
    CCamera* _pCam = CRenderMgr::GetInst()->GetMainCamera();

    if (nullptr == _pCam)
    {
        return Vec2(-100, -100);
    }

    Matrix VPMatrix = _pCam->GetViewMat() * _pCam->GetProjMat();

    // WolrdPos -> NDC
    Vec4 TargetNDCPos = Vector4::Transform(Vec4(TargetPos.x, TargetPos.y, TargetPos.z, 1.f), VPMatrix);

    // Perspective Division
    TargetNDCPos.x /= TargetNDCPos.w;
    TargetNDCPos.y /= TargetNDCPos.w;
    TargetNDCPos.z /= TargetNDCPos.w;

    return Vec2(TargetNDCPos.x, TargetNDCPos.y);
}

UINT CFadeOutScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_Duration, sizeof(float), 1, _File);
    fwrite(&m_RotateSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    return MemoryByte;
}

UINT CFadeOutScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_Duration, sizeof(float), 1, _File);
    fread(&m_RotateSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);
    return MemoryByte;
}
