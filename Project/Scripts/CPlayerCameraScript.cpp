#include "pch.h"
#include "CPlayerCameraScript.h"
#include <Engine\\CLevelMgr.h>

CPlayerCameraScript::CPlayerCameraScript()
    : CScript(PLAYERCAMERASCRIPT)
    , m_Player(nullptr)
    , m_OffsetPos()
    , m_Speed(10.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_OffsetPos, "Offset Position");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
}

CPlayerCameraScript::CPlayerCameraScript(const CPlayerCameraScript& origin)
    : CScript(origin)
    , m_Player(nullptr)
    , m_OffsetPos(origin.m_OffsetPos)
    , m_Speed(origin.m_Speed)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_OffsetPos, "Offset Position");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
}

CPlayerCameraScript::~CPlayerCameraScript()
{
}

void CPlayerCameraScript::begin()
{
    m_Player = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
}

void CPlayerCameraScript::tick()
{
    if (nullptr == m_Player)
        return;

    Vec3 CamPos = Transform()->GetRelativePos();
    Vec3 PlayerPos = m_Player->Transform()->GetWorldPos();

    CamPos = Vec3::Lerp(CamPos - m_OffsetPos, PlayerPos, DT * m_Speed);
    Transform()->SetRelativePos(CamPos + m_OffsetPos);
}

void CPlayerCameraScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_OffsetPos, 1, sizeof(Vec3), _File);
    fwrite(&m_Speed, 1, sizeof(float), _File);
}

void CPlayerCameraScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_OffsetPos, 1, sizeof(Vec3), _File);
    fread(&m_Speed, 1, sizeof(float), _File);
}
