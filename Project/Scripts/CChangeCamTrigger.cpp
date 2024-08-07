#include "pch.h"
#include "CChangeCamTrigger.h"

#include "CCameraController.h"

CChangeCamTrigger::CChangeCamTrigger()
    : CScript(CHANGECAMTRIGGER)
    , m_bIsReturn(false)
    , m_bChangeOffset(false)
    , m_bChangedir(false)
    , m_bChangeDist(false)
    , m_bChangeRotationSpeed(false)
    , m_bChangeZoomspeed(false)
    , m_ChangeOffset(Vec3(0.f,0.f,0.f))
    , m_ChangeLookDir(Vec3(0.f, 0.f, 0.f))
    , m_ChangeLookDist(1500.f)
    , m_RotationSpeed(50.f)
    , m_ZoomSpeed(500.f)
    , m_UndoOffset(Vec3(0.f,0.f,0.f))
    , m_UndoDir(Vec3(0.f, 0.f, 0.f))
    , m_UndoDist(1500.f)
    , m_UndoRotataionSpeed(50.f)
    , m_UndoZoomSpeed(500.f)

{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bIsReturn, "Setting Return");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeOffset, "Change Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ChangeOffset, "Offset");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangedir, "Change Direction");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ChangeLookDir, "Direction");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeDist, "Change Distance");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ChangeLookDist, "Look Distance");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeRotationSpeed, "Change Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotationSpeed, "Rotation Speed");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeZoomspeed, "Change Zoom Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ZoomSpeed, "Zoom Speed");
}

CChangeCamTrigger::CChangeCamTrigger(const CChangeCamTrigger& _Origin)
    : CScript(CHANGECAMTRIGGER)
    , m_bIsReturn(_Origin.m_bIsReturn)
    , m_bChangeOffset(_Origin.m_bChangeOffset)
    , m_bChangedir(_Origin.m_bChangedir)
    , m_bChangeDist(_Origin.m_bChangeDist)
    , m_bChangeRotationSpeed(_Origin.m_bChangeRotationSpeed)
    , m_bChangeZoomspeed(_Origin.m_bChangeZoomspeed)
    , m_ChangeOffset(_Origin.m_ChangeOffset)
    , m_ChangeLookDir(_Origin.m_ChangeLookDir)
    , m_ChangeLookDist(_Origin.m_ChangeLookDist)
    , m_RotationSpeed(_Origin.m_RotationSpeed)
    , m_ZoomSpeed(_Origin.m_ZoomSpeed)
    , m_UndoOffset(_Origin.m_UndoOffset)
    , m_UndoDir(_Origin.m_UndoDir)
    , m_UndoDist(_Origin.m_UndoDist)
    , m_UndoRotataionSpeed(_Origin.m_UndoRotataionSpeed)
    , m_UndoZoomSpeed(_Origin.m_UndoZoomSpeed)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bIsReturn, "Setting Return");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeOffset, "Change Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ChangeOffset, "Offset");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangedir, "Change Direction");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ChangeLookDir, "Direction");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeDist, "Change Distance");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ChangeLookDist, "Look Distance");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeRotationSpeed, "Change Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_RotationSpeed, "Rotation Speed");

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChangeZoomspeed, "Change Zoom Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ZoomSpeed, "Zoom Speed");
}

CChangeCamTrigger::~CChangeCamTrigger()
{
}

void CChangeCamTrigger::begin()
{
    m_MainCamController = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera")->GetScript<CCameraController>();
}


void CChangeCamTrigger::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_PLAYER)
        return;

    if (m_MainCamController == nullptr)
        return;

    if (m_bIsReturn)
    {
        if (m_bChangeOffset)
        {
            m_UndoOffset = m_MainCamController->GetOffset();
        }

        if (m_bChangedir)
        {
            m_UndoDir = m_MainCamController->GetLookDir();
        }
        if (m_bChangeDist)
        {
            m_UndoDist = m_MainCamController->GetLookDist();
        }
        if (m_bChangeRotationSpeed)
        {
            m_UndoRotataionSpeed = m_MainCamController->GetRotationSpeed();
        }
        if (m_bChangeZoomspeed)
        {
            m_UndoZoomSpeed = m_MainCamController->GetZoomMaxSpeed();
        }
    }

    if (m_bChangeOffset)
    {
        m_MainCamController->SetOffset(m_ChangeOffset);
    }

    if (m_bChangedir)
    {
        m_MainCamController->SetLookDir(m_ChangeLookDir);
    }

    if (m_bChangeDist)
    {
        m_MainCamController->SetLookDist(m_ChangeLookDist);
    }

    if (m_bChangeRotationSpeed)
    {
        m_MainCamController->SetRotationSpeed(m_RotationSpeed);
    }
    if (m_bChangeZoomspeed)
    {
        m_MainCamController->SetZoomMaxSpeed(m_ZoomSpeed);
    }
}

void CChangeCamTrigger::OnTriggerExit(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_PLAYER)
        return;

    if (false == m_bIsReturn)
        return;

    if (m_bChangeOffset)
    {
        m_MainCamController->SetOffset(m_UndoOffset);
    }

    if (m_bChangedir)
    {
        m_MainCamController->SetLookDir(m_UndoDir);
    }

    if (m_bChangeDist)
    {
        m_MainCamController->SetLookDist(m_UndoDist);
    }
    if (m_bChangeRotationSpeed)
    {
        m_MainCamController->SetRotationSpeed(m_UndoRotataionSpeed);
    }
    if (m_bChangeZoomspeed)
    {
        m_MainCamController->SetZoomMaxSpeed(m_UndoZoomSpeed);
    }
}

UINT CChangeCamTrigger::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bIsReturn, sizeof(bool), 1, _File);

    fwrite(&m_bChangeOffset, sizeof(bool), 1, _File);
    fwrite(&m_ChangeOffset, sizeof(Vec3), 1, _File);

    fwrite(&m_bChangedir, sizeof(bool), 1, _File);
    fwrite(&m_ChangeLookDir, sizeof(Vec3), 1, _File);

    fwrite(&m_bChangeDist, sizeof(bool), 1, _File);
    fwrite(&m_ChangeLookDist, sizeof(float), 1, _File);

    fwrite(&m_bChangeRotationSpeed, sizeof(bool), 1, _File);
    fwrite(&m_RotationSpeed, sizeof(float), 1, _File);

    fwrite(&m_bChangeZoomspeed, sizeof(bool), 1, _File);
    fwrite(&m_ZoomSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(bool);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CChangeCamTrigger::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bIsReturn, sizeof(bool), 1, _File);

    fread(&m_bChangeOffset, sizeof(bool), 1, _File);
    fread(&m_ChangeOffset, sizeof(Vec3), 1, _File);

    fread(&m_bChangedir, sizeof(bool), 1, _File);
    fread(&m_ChangeLookDir, sizeof(Vec3), 1, _File);

    fread(&m_bChangeDist, sizeof(bool), 1, _File);
    fread(&m_ChangeLookDist, sizeof(float), 1, _File);

    fread(&m_bChangeRotationSpeed, sizeof(bool), 1, _File);
    fread(&m_RotationSpeed, sizeof(float), 1, _File);

    fread(&m_bChangeZoomspeed, sizeof(bool), 1, _File);
    fread(&m_ZoomSpeed, sizeof(float), 1, _File);

    MemoryByte += sizeof(bool);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
