#include "pch.h"
#include "CTwoTargetCamTrigger.h"

#include "CCameraController.h"

CTwoTargetCamTrigger::CTwoTargetCamTrigger()
    : CScript(TWOTARGETCAMTRIGGER)
    , m_SubTargetName("Defualt")
    , m_bChangeDir(true)
    , m_LookDir(Vec3(0.f,-1.f,0.f))
    , m_bChangeRotationSpeed(false)
    , m_bChangeZoomspeed(false)
    , m_RotationSpeed(50.f)
    , m_ZoomSpeed(500.f)
    , m_UndoRotataionSpeed(50.f)
    , m_UndoZoomSpeed(500.f)
    , m_MinDist(300.f)
    , m_DistanceOffset(500.f)
    , m_ExitDir(Vec3(0.f,-1.f,1.f))
    , m_ExitDistance(1500.f)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_SubTargetName, "Target Name");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeDir, "Change Dir");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_LookDir, "Look Dir");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeRotationSpeed, "Change Rot Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotationSpeed, "Rot Speed");


    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeZoomspeed, "Change Zoom Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ZoomSpeed, "Zoom Speed");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinDist, "MinDistance");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_DistanceOffset, "Distance Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ExitDir, "Exit Dir");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ExitDistance, "Exit Distance");
}

CTwoTargetCamTrigger::CTwoTargetCamTrigger(const CTwoTargetCamTrigger& _Origin)
    : CScript(TWOTARGETCAMTRIGGER)
    , m_SubTargetName(_Origin.m_SubTargetName)
    , m_bChangeDir(_Origin.m_bChangeDir)
    , m_LookDir(_Origin.m_LookDir)
    , m_bChangeRotationSpeed(_Origin.m_bChangeRotationSpeed)
    , m_bChangeZoomspeed(_Origin.m_bChangeZoomspeed)
    , m_RotationSpeed(_Origin.m_RotationSpeed)
    , m_ZoomSpeed(_Origin.m_ZoomSpeed)
    , m_UndoRotataionSpeed(_Origin.m_UndoRotataionSpeed)
    , m_UndoZoomSpeed(_Origin.m_UndoZoomSpeed)
    , m_MinDist(_Origin.m_MinDist)
    , m_DistanceOffset(_Origin.m_DistanceOffset)
    , m_ExitDir(_Origin.m_ExitDir)
    , m_ExitDistance(_Origin.m_ExitDistance)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_SubTargetName, "Target Name");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeDir, "Change Dir");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_LookDir, "Look Dir");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeRotationSpeed, "Change Rot Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotationSpeed, "Rot Speed");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeZoomspeed, "Change Zoom Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ZoomSpeed, "Zoom Speed");

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MinDist, "MinDistance");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_DistanceOffset, "Distance Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ExitDir, "Exit Dir");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ExitDistance, "Exit Distance");
}

CTwoTargetCamTrigger::~CTwoTargetCamTrigger()
{
}


void CTwoTargetCamTrigger::begin()
{
    m_MainCamController = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera")->GetScript<CCameraController>();

}

void CTwoTargetCamTrigger::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_PLAYER)
        return;

    if (m_MainCamController == nullptr)
        return;

    wstring SubTargetName = ToWstring(m_SubTargetName);

    CGameObject* SubTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(SubTargetName);

    if (SubTarget == nullptr)
        return;

    m_MainCamController->TwoTarget(SubTarget, m_bChangeDir, m_LookDir, m_DistanceOffset, m_MinDist);

    if (m_bChangeRotationSpeed)
    {
        m_UndoRotataionSpeed = m_MainCamController->GetRotationSpeed();
        m_MainCamController->SetRotationSpeed(m_RotationSpeed);
    }

    if (m_bChangeZoomspeed)
    {
        m_UndoZoomSpeed = m_MainCamController->GetZoomMaxSpeed();
        m_MainCamController->SetZoomMaxSpeed(m_ZoomSpeed);
    }

}

void CTwoTargetCamTrigger::OnTriggerExit(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_PLAYER)
        return;

    if (m_MainCamController == nullptr)
        return;

    if (m_MainCamController->GetCameraSetup() != CameraSetup::TWOTARGET)
        return;

    m_MainCamController->SetCameraSetup(CameraSetup::NORMAL);
    m_MainCamController->SetDistanceOffset(0.f);
    m_MainCamController->SetSubTarget(nullptr);

    m_MainCamController->SetLookDir(m_ExitDir);
    m_MainCamController->SetLookDist(m_ExitDistance);

    if (m_bChangeRotationSpeed)
    {
        m_MainCamController->SetRotationSpeed(m_UndoRotataionSpeed);
    }

    if (m_bChangeZoomspeed)
    {
        m_MainCamController->SetZoomMaxSpeed(m_UndoZoomSpeed);
    }
}

UINT CTwoTargetCamTrigger::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += SaveWStringToFile(ToWstring(m_SubTargetName), _File);

    fwrite(&m_bChangeDir, sizeof(bool), 1, _File);
    fwrite(&m_LookDir, sizeof(Vec3), 1, _File);

    fwrite(&m_bChangeRotationSpeed, sizeof(bool), 1, _File);
    fwrite(&m_RotationSpeed, sizeof(float), 1, _File);
    fwrite(&m_UndoRotataionSpeed, sizeof(float), 1, _File);

    fwrite(&m_bChangeZoomspeed, sizeof(bool), 1, _File);
    fwrite(&m_ZoomSpeed, sizeof(float), 1, _File);
    fwrite(&m_UndoZoomSpeed, sizeof(float), 1, _File);

    fwrite(&m_MinDist, sizeof(float), 1, _File);
    fwrite(&m_DistanceOffset, sizeof(float), 1, _File);

    fwrite(&m_ExitDir, sizeof(Vec3), 1, _File);
    fwrite(&m_ExitDistance, sizeof(float), 1, _File);


    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CTwoTargetCamTrigger::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring tmp;
    MemoryByte += LoadWStringFromFile(tmp, _File);
    m_SubTargetName = ToString(tmp);

    fread(&m_bChangeDir, sizeof(bool), 1, _File);
    fread(&m_LookDir, sizeof(Vec3), 1, _File);

    fread(&m_bChangeRotationSpeed, sizeof(bool), 1, _File);
    fread(&m_RotationSpeed, sizeof(float), 1, _File);
    fread(&m_UndoRotataionSpeed, sizeof(float), 1, _File);

    fread(&m_bChangeZoomspeed, sizeof(bool), 1, _File);
    fread(&m_ZoomSpeed, sizeof(float), 1, _File);
    fread(&m_UndoZoomSpeed, sizeof(float), 1, _File);

    fread(&m_MinDist, sizeof(float), 1, _File);
    fread(&m_DistanceOffset, sizeof(float), 1, _File);

    fread(&m_ExitDir, sizeof(Vec3), 1, _File);
    fread(&m_ExitDistance, sizeof(float), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
