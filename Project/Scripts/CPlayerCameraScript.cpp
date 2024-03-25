#include "pch.h"
#include "CPlayerCameraScript.h"
#include "CPlayerScript.h"

CPlayerCameraScript::CPlayerCameraScript()
    : CScript(PLAYERCAMERASCRIPT)
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

        if (fabsf(pos.x - PlayerPos.x) > m_CamMoveRangeX)
            pos.x += (Dir.Normalize() * m_CamSpeed * DT).x;

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

        Transform()->SetRelativePos(pos + m_OffsetPos);
    }
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
