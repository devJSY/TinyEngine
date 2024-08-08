#include "pch.h"
#include "CProgressCamTrigger.h"

#include "CCameraController.h"

CProgressCamTrigger::CProgressCamTrigger()
    : CScript(PROGRESSCAMTRIGGER)
    , m_MainCamController(nullptr)
    , m_StartPos(Vec3(0.f, 0.f, 0.f))
    , m_EndPos(Vec3(0.f, 0.f, 0.f))
    , m_StartOffset(Vec3(0.f, 0.f, 0.f))
    , m_EndOffset(Vec3(0.f, 0.f, 0.f))
    , m_StartDir(Vec3(0.f, 0.f, 0.f))
    , m_EndDir(Vec3(0.f, 0.f, 0.f))
    , m_StartDist(0.f)
    , m_EndDist(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_StartOffset, "Start Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_EndOffset, "End Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_StartDir, "Start Direction");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_EndDir, "End Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_StartDist, "Start Distance");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_EndDist, "End Distance");
}

CProgressCamTrigger::CProgressCamTrigger(const CProgressCamTrigger& _Origin)
    : CScript(PROGRESSCAMTRIGGER)
    , m_MainCamController(nullptr)
    , m_StartPos(_Origin.m_StartPos)
    , m_EndPos(_Origin.m_EndPos)
    , m_StartOffset(_Origin.m_StartOffset)
    , m_EndOffset(_Origin.m_EndOffset)
    , m_StartDir(_Origin.m_StartDir)
    , m_EndDir(_Origin.m_EndDir)
    , m_StartDist(_Origin.m_StartDist)
    , m_EndDist(_Origin.m_EndDist)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_StartOffset, "Start Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_EndOffset, "End Offset");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_StartDir, "Start Direction");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_EndDir, "End Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_StartDist, "Start Distance");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_EndDist, "End Distance");
}

CProgressCamTrigger::~CProgressCamTrigger()
{
}

void CProgressCamTrigger::begin()
{
    m_MainCamController = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera")->GetScript<CCameraController>();

    if (m_MainCamController == nullptr)
        return;

    Vec3 OwnerPos = Transform()->GetWorldPos();
    Vec3 Front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    float Scale = Transform()->GetWorldScale().z;

    m_StartPos = OwnerPos + (Scale * -Front);
    m_EndPos = OwnerPos + (Scale * Front);
}

void CProgressCamTrigger::tick()
{
#ifndef DISTRIBUTE

    // Debug
    // Start (Blue)
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), -Transform()->GetWorldDir(DIR_TYPE::FRONT), Transform()->GetWorldScale().z,
                                  Vec3(0.f, 0.f, 1.f), true);

    // End (Red)
    GamePlayStatic::DrawDebugLine(Transform()->GetWorldPos(), Transform()->GetWorldDir(DIR_TYPE::FRONT), Transform()->GetWorldScale().z,
                                  Vec3(1.f, 0.f, 0.f), true);
#endif


}

void CProgressCamTrigger::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_PLAYER)
        return;

    if (m_MainCamController == nullptr)
        return;

    m_MainCamController->ProgressSetup(m_StartPos, m_EndPos, m_StartOffset, m_EndOffset, m_StartDir, m_EndDir, m_StartDist, m_EndDist);
}

void CProgressCamTrigger::OnTriggerExit(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_PLAYER)
        return;

    if (m_MainCamController == nullptr)
        return;

    m_MainCamController->SetCameraSetup(CameraSetup::NORMAL);

    Vec3 TargetPos = m_MainCamController->GetMainTarget()->Transform()->GetWorldPos();

    Vec3 ProgressRoad = m_EndPos - m_StartPos;
    Vec3 ToPrevLookAt = TargetPos - m_StartPos;

    float DotRes = ToPrevLookAt.Dot(ProgressRoad);
    float LengthSquared = ProgressRoad.Dot(ProgressRoad);

    // Start 쪽으로 나간경우
    if (DotRes <= 0.f)
    {
        m_MainCamController->SetOffset(m_StartOffset);
        m_MainCamController->SetLookDir(m_StartDir);
        m_MainCamController->SetLookDist(m_StartDist);
    }

    // End 쪽으로 나간경우
    if (DotRes > LengthSquared)
    {
        m_MainCamController->SetOffset(m_EndOffset);
        m_MainCamController->SetLookDir(m_EndDir);
        m_MainCamController->SetLookDist(m_EndDist);
    }
}

UINT CProgressCamTrigger::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_StartOffset, sizeof(Vec3), 1, _File);
    fwrite(&m_EndOffset, sizeof(Vec3), 1, _File);
    fwrite(&m_StartDir, sizeof(Vec3), 1, _File);
    fwrite(&m_EndDir, sizeof(Vec3), 1, _File);
    fwrite(&m_StartDist, sizeof(float), 1, _File);
    fwrite(&m_EndDist, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CProgressCamTrigger::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_StartOffset, sizeof(Vec3), 1, _File);
    fread(&m_EndOffset, sizeof(Vec3), 1, _File);
    fread(&m_StartDir, sizeof(Vec3), 1, _File);
    fread(&m_EndDir, sizeof(Vec3), 1, _File);
    fread(&m_StartDist, sizeof(float), 1, _File);
    fread(&m_EndDist, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
