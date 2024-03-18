#include "pch.h"
#include "CPlayerCameraScript.h"
#include "CPlayerScript.h"

CPlayerCameraScript::CPlayerCameraScript()
    : CScript(PLAYERCAMERASCRIPT)
    , m_CamSpeed(1000.f)
    , m_CamMoveRange(0.f)
    , m_OffsetPos()
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamSpeed, "Camera Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamMoveRange, "Camera Move Range");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_OffsetPos, "Offset Position");
}

CPlayerCameraScript::~CPlayerCameraScript()
{
}

void CPlayerCameraScript::tick()
{
    CGameObject* pPlayer = GetOwner()->GetParent();
    if (nullptr != pPlayer)
    {
        Vec3 pos = Transform()->GetRelativePos() - m_OffsetPos;
        Vec3 PlayerPos = pPlayer->Transform()->GetRelativePos();
        Vec3 Dir = PlayerPos - pos;

        if (Dir.Length() > m_CamMoveRange)
        {
            pos += Dir.Normalize() * m_CamSpeed * DT;
        }

        Transform()->SetRelativePos(pos + m_OffsetPos);
    }
}

void CPlayerCameraScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_CamSpeed, sizeof(float), 1, _File);
    fwrite(&m_CamMoveRange, sizeof(float), 1, _File);
    fwrite(&m_OffsetPos, sizeof(Vec3), 1, _File);
}

void CPlayerCameraScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_CamSpeed, sizeof(float), 1, _File);
    fread(&m_CamMoveRange, sizeof(float), 1, _File);
    fread(&m_OffsetPos, sizeof(Vec3), 1, _File);
}
