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

    Vec3 pos = Transform()->GetRelativePos();
    Vec3 PlayerPos = m_Player->Transform()->GetRelativePos();
    pos.z = PlayerPos.z; // z축 플레이어 위치 기준으로 설정하여 연산

    pos = Vec3::Lerp(pos, PlayerPos + m_OffsetPos, DT * m_CamSpeed);

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

    pos.z = 0.f; // 플레이어 카메라 Z축은 항상 0으로 설정
    Transform()->SetRelativePos(pos);
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
