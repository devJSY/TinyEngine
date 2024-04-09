#include "pch.h"
#include "CPlayerCameraScript.h"
#include "CPlayerScript.h"

CPlayerCameraScript::CPlayerCameraScript()
    : CScript(PLAYERCAMERASCRIPT)
    , m_Player(nullptr)
    , m_CamSpeed(1000.f)
    , m_CamMoveRangeX(0.f)
    , m_CamMoveRangeY(0.f)
    , m_OffsetPos()
    , m_listCamEffect{}
    , m_bLocked(false)
    , m_LockedPos(Vec3())
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamSpeed, "Camera Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamMoveRangeX, "Camera Move Range X");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamMoveRangeY, "Camera Move Range Y");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_OffsetPos, "Offset Position");
}

CPlayerCameraScript::CPlayerCameraScript(const CPlayerCameraScript& origin)
    : CScript(origin)
    , m_Player(origin.m_Player)
    , m_CamSpeed(origin.m_CamSpeed)
    , m_CamMoveRangeX(origin.m_CamMoveRangeX)
    , m_CamMoveRangeY(origin.m_CamMoveRangeY)
    , m_OffsetPos(origin.m_OffsetPos)
    , m_listCamEffect(origin.m_listCamEffect)
    , m_bLocked(origin.m_bLocked)
    , m_LockedPos(origin.m_LockedPos)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamSpeed, "Camera Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamMoveRangeX, "Camera Move Range X");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CamMoveRangeY, "Camera Move Range Y");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_OffsetPos, "Offset Position");
}

CPlayerCameraScript::~CPlayerCameraScript()
{
}

void CPlayerCameraScript::begin()
{
    m_Player = GetOwner()->GetParent();

    if (nullptr != m_Player)
    {
        Vec3 PlayerPos = m_Player->Transform()->GetRelativePos();
        PlayerPos.z = 0.f;
        Transform()->SetRelativePos(PlayerPos);
    }
}

void CPlayerCameraScript::tick()
{
    if (nullptr == m_Player)
        return;

    Vec3 pos = Transform()->GetRelativePos();

    if (m_bLocked)
    {
        pos.z = m_LockedPos.z;
        pos = Vec3::Lerp(pos, m_LockedPos, DT * 2.f);
    }
    else
    {
        Vec3 PlayerPos = m_Player->Transform()->GetRelativePos();
        pos.z = PlayerPos.z; // z축 플레이어 위치 기준으로 설정하여 연산

        pos = Vec3::Lerp(pos, PlayerPos + m_OffsetPos, DT * m_CamSpeed);
    }

    // Camera Effect
    if (!m_listCamEffect.empty())
    {
        tCamEffect& effect = m_listCamEffect.front();

        effect.fCurTime += DT;

        if (effect.eShakeDir == ShakeDir::Horizontal)
        {
            pos.x += GetRandomInt(-1, 1) * effect.fMagnitude;
        }
        else if (effect.eShakeDir == ShakeDir::Vertical)
        {
            pos.y += GetRandomInt(-1, 1) * effect.fMagnitude;
        }
        else if (effect.eShakeDir == ShakeDir::Comprehensive)
        {
            pos.x += GetRandomInt(-1, 1) * effect.fMagnitude;
            pos.y += GetRandomInt(-1, 1) * effect.fMagnitude;
        }

        if (effect.fCurTime > effect.fDuration)
        {
            m_listCamEffect.pop_front();
        }
    }

    pos.z = 0.f; // 플레이어 카메라 Z축은 항상 0으로 설정
    Transform()->SetRelativePos(pos);
}

void CPlayerCameraScript::Lock(bool _bLocked, Vec3 _LockedPos)
{
    m_bLocked = _bLocked;
    m_LockedPos = _LockedPos;
}

void CPlayerCameraScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_CamSpeed, sizeof(float), 1, _File);
    fwrite(&m_CamMoveRangeX, sizeof(float), 1, _File);
    fwrite(&m_CamMoveRangeY, sizeof(float), 1, _File);
    fwrite(&m_OffsetPos, sizeof(Vec3), 1, _File);
    fwrite(&m_bLocked, sizeof(bool), 1, _File);
    fwrite(&m_LockedPos, sizeof(Vec3), 1, _File);
}

void CPlayerCameraScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_CamSpeed, sizeof(float), 1, _File);
    fread(&m_CamMoveRangeX, sizeof(float), 1, _File);
    fread(&m_CamMoveRangeY, sizeof(float), 1, _File);
    fread(&m_OffsetPos, sizeof(Vec3), 1, _File);
    fread(&m_bLocked, sizeof(bool), 1, _File);
    fread(&m_LockedPos, sizeof(Vec3), 1, _File);
}
