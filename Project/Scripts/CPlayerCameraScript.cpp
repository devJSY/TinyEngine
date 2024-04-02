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

    Vec3 pos = Transform()->GetRelativePos() - m_OffsetPos;
    Vec3 PlayerPos = m_Player->Transform()->GetRelativePos();
    pos.z = PlayerPos.z;
    Vec3 Dir = PlayerPos - pos;

    if (PLAYER_STATE::Dash == m_Player->GetScript<CPlayerScript>()->GetState())
    {
        pos.x += (Dir.Normalize() * m_CamSpeed * 2.5f * DT).x;
    }
    else if (PLAYER_STATE::DownAttack == m_Player->GetScript<CPlayerScript>()->GetState())
    {
        pos.y += (Dir.Normalize() * m_CamSpeed * 2.5f * DT).y;
    }
    else
    {
        if (fabsf(pos.x - PlayerPos.x) > m_CamMoveRangeX)
            pos.x += (Dir.Normalize() * m_CamSpeed * DT).x;
    }

    if (fabsf(pos.y - PlayerPos.y) > m_CamMoveRangeY)
        pos.y += (Dir.Normalize() * m_CamSpeed * DT).y;

    // Camera Effect
    if (!m_listCamEffect.empty())
    {
        tCamEffect& effect = m_listCamEffect.front();

        effect.fCurTime += DT;

        if (effect.eShakeDir == ShakeDir::Horizontal)
        {
            pos.x += cosf(effect.fCurTime * effect.fSpeed) * effect.fDistance;
        }
        else if (effect.eShakeDir == ShakeDir::Vertical)
        {
            pos.y += -sinf(effect.fCurTime * effect.fSpeed) * effect.fDistance;
        }
        else if (effect.eShakeDir == ShakeDir::Comprehensive)
        {
            pos.x += cosf(effect.fCurTime * effect.fSpeed) * effect.fDistance;
            pos.y -= sinf(effect.fCurTime * effect.fSpeed) * effect.fDistance;
        }

        if (effect.fCurTime > effect.fDuration)
        {
            m_listCamEffect.pop_front();
        }
    }

    pos.z = 0.f;
    Transform()->SetRelativePos(pos + m_OffsetPos);
}

void CPlayerCameraScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_CamSpeed, sizeof(float), 1, _File);
    fwrite(&m_CamMoveRangeX, sizeof(float), 1, _File);
    fwrite(&m_CamMoveRangeY, sizeof(float), 1, _File);
    fwrite(&m_OffsetPos, sizeof(Vec3), 1, _File);
}

void CPlayerCameraScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_CamSpeed, sizeof(float), 1, _File);
    fread(&m_CamMoveRangeX, sizeof(float), 1, _File);
    fread(&m_CamMoveRangeY, sizeof(float), 1, _File);
    fread(&m_OffsetPos, sizeof(Vec3), 1, _File);
}
