#include "pch.h"
#include "CPlayerCameraScript.h"

CPlayerCameraScript::CPlayerCameraScript()
    : CScript(PLAYERCAMERASCRIPT)
    , m_OffsetPos()
{
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
        Transform()->SetRelativePos(pPlayer->Transform()->GetRelativePos() + m_OffsetPos);
    }
}

void CPlayerCameraScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_OffsetPos, sizeof(Vec3), 1, _File);
}

void CPlayerCameraScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_OffsetPos, sizeof(Vec3), 1, _File);
}
