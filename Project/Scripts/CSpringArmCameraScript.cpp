#include "pch.h"
#include "CSpringArmCameraScript.h"
#include <Engine\CRenderMgr.h>

CSpringArmCameraScript::CSpringArmCameraScript()
    : CScript(SPRINGARMCAMERASCRIPT)
    , m_ArmLength(50.f)
    , m_LagSpeed(0.f)
    , m_LagMaxDistance(0.f)
    , m_bUnableLag(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_CameraOffset, "Camera Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_TargetOffset, "Target Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ArmRotation, "Arm Rotation");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ArmLength, "Arm Length");
}

CSpringArmCameraScript::CSpringArmCameraScript(const CSpringArmCameraScript& _Origin)
    : CScript(SPRINGARMCAMERASCRIPT)
    , m_CameraOffset(_Origin.m_CameraOffset)
    , m_TargetOffset(_Origin.m_TargetOffset)
    , m_ArmRotation(_Origin.m_ArmRotation)
    , m_ArmLength(_Origin.m_ArmLength)
    , m_LagSpeed(_Origin.m_LagSpeed)
    , m_LagMaxDistance(_Origin.m_LagMaxDistance)
    , m_bUnableLag(_Origin.m_bUnableLag)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_CameraOffset, "Camera Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_TargetOffset, "Target Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ArmRotation, "Arm Rotation");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ArmLength, "Arm Length");
}

CSpringArmCameraScript::~CSpringArmCameraScript()
{
}

void CSpringArmCameraScript::begin()
{
}

void CSpringArmCameraScript::tick()
{
    if (!GetOwner()->GetParent())
        return;

    Vec3 RadRot = Vec3(XMConvertToRadians(m_ArmRotation.x), XMConvertToRadians(m_ArmRotation.y), XMConvertToRadians(m_ArmRotation.z));
    Vec3 TargetPos = GetOwner()->GetParent()->Transform()->GetLocalPos() + m_TargetOffset;
    
    // get spring arm world matrix
    Matrix matSpringRotX = XMMatrixRotationX(RadRot.x);
    Matrix matSpringRotY = XMMatrixRotationY(RadRot.y);
    Matrix matSpringRotZ = XMMatrixRotationZ(RadRot.z);
    Matrix matSpringTranslation = XMMatrixTranslation(m_TargetOffset.x, m_TargetOffset.y, m_TargetOffset.z);
    Matrix matSpringTransform = matSpringRotX * matSpringRotY * matSpringRotZ * matSpringTranslation;

    // get camera world matrix
    const static Matrix matCamRotX = XMMatrixRotationX(0.f);
    const static Matrix matCamRotY = XMMatrixRotationY(0.f);
    const static Matrix matCamRotZ = XMMatrixRotationZ(0.f);
    Matrix matCamTranslation = XMMatrixTranslation(m_CameraOffset.x, m_CameraOffset.y, m_CameraOffset .z- m_ArmLength);
    Matrix matCamTransform = matCamRotX * matCamRotY * matCamRotZ * matCamTranslation;

    matCamTransform *= matSpringTransform;

    // set camera
    Vec3 NewCamPos = matCamTransform.Translation();

    GetOwner()->Transform()->SetLocalPos(NewCamPos);
    GetOwner()->Transform()->SetLocalRotation(RadRot);

    // draw debug
    if (CRenderMgr::GetInst()->IsShowDebugRender())
    {
        NewCamPos = GetOwner()->Transform()->GetWorldPos();
        NewCamPos -= XMVector3TransformNormal(m_CameraOffset, matCamTransform);
        GamePlayStatic::DrawDebugLine(TargetPos, (NewCamPos - TargetPos).Normalize(), m_ArmLength, Vec3(1, 0, 0), false);
    }
}

void CSpringArmCameraScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_CameraOffset, 1, sizeof(Vec3), _File);
    fwrite(&m_TargetOffset, 1, sizeof(Vec3), _File);
    fwrite(&m_ArmRotation, 1, sizeof(Vec3), _File);
    fwrite(&m_ArmLength, 1, sizeof(float), _File);
    fwrite(&m_LagSpeed, 1, sizeof(float), _File);
    fwrite(&m_LagMaxDistance, 1, sizeof(float), _File);
    fwrite(&m_bUnableLag, 1, sizeof(bool), _File);
}

void CSpringArmCameraScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_CameraOffset, 1, sizeof(Vec3), _File);
    fread(&m_TargetOffset, 1, sizeof(Vec3), _File);
    fread(&m_ArmRotation, 1, sizeof(Vec3), _File);
    fread(&m_ArmLength, 1, sizeof(float), _File);
    fread(&m_LagSpeed, 1, sizeof(float), _File);
    fread(&m_LagMaxDistance, 1, sizeof(float), _File);
    fread(&m_bUnableLag, 1, sizeof(bool), _File);
}
