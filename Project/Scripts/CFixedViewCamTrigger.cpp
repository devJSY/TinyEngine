#include "pch.h"
#include "CFixedViewCamTrigger.h"

#include "CPlayerMgr.h"
#include "CCameraController.h"

CFixedViewCamTrigger::CFixedViewCamTrigger()
    : CScript(FIXEDVIEWCAMTRIGGER)
    , m_bIsImmediate(false)
    , m_FixedViewPos(Vec3(0.f, 0.f, 0.f))
    , m_Undo(false)
    , m_ExitLookDir(Vec3(0.f, 0.f, 0.f))
    , m_ExitLookDist(2000.f)
    , m_bExitImmediate(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bIsImmediate, "Immediate Change");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_FixedViewPos, "Fixed Cam Position");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_Undo, "Undo");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ExitLookDir, "Exit LookDir");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ExitLookDist, "Exit LookDist");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bExitImmediate, "Exit Immediate");
}

CFixedViewCamTrigger::CFixedViewCamTrigger(const CFixedViewCamTrigger& _Origin)
    : CScript(FIXEDVIEWCAMTRIGGER)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bIsImmediate, "Immediate Change");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_FixedViewPos, "Fixed Cam Position");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_Undo, "Undo");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_ExitLookDir, "Exit LookDir");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ExitLookDist, "Exit LookDist");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bExitImmediate, "Exit Immediate");
}

CFixedViewCamTrigger::~CFixedViewCamTrigger()
{
}

void CFixedViewCamTrigger::begin()
{
    m_MainCamController = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera")->GetScript<CCameraController>();
}

void CFixedViewCamTrigger::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_PLAYER)
        return;

    if (m_MainCamController == nullptr)
        return;

    CAMERACTRL->FixedView(m_bIsImmediate, m_FixedViewPos);
}

void CFixedViewCamTrigger::OnTriggerExit(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_PLAYER)
        return;

    if (m_MainCamController == nullptr)
        return;

    CAMERACTRL->Normal(m_bExitImmediate);

    CAMERACTRL->SetLookDir(m_ExitLookDir);
    CAMERACTRL->SetLookDist(m_ExitLookDist);
}

UINT CFixedViewCamTrigger::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bIsImmediate, sizeof(bool), 1, _File);
    fwrite(&m_FixedViewPos, sizeof(Vec3), 1, _File);
    fwrite(&m_Undo, sizeof(bool), 1, _File);
    fwrite(&m_ExitLookDir, sizeof(Vec3), 1, _File);
    fwrite(&m_ExitLookDist, sizeof(float), 1, _File);
    fwrite(&m_bExitImmediate, sizeof(bool), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CFixedViewCamTrigger::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bIsImmediate, sizeof(bool), 1, _File);
    fread(&m_FixedViewPos, sizeof(Vec3), 1, _File);
    fread(&m_Undo, sizeof(bool), 1, _File);
    fread(&m_ExitLookDir, sizeof(Vec3), 1, _File);
    fread(&m_ExitLookDist, sizeof(float), 1, _File);
    fread(&m_bExitImmediate, sizeof(bool), 1, _File);

    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}
