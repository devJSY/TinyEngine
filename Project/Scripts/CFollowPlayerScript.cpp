#include "pch.h"
#include "CFollowPlayerScript.h"
#include "CPlayerMgr.h"

CFollowPlayerScript::CFollowPlayerScript()
    : CScript(FOLLOWPLAYERSCRIPT)
    , m_LightDir{}
    , m_Dist(1500.f)
{
}

CFollowPlayerScript::CFollowPlayerScript(const CFollowPlayerScript& _Origin)
    : CScript(FOLLOWPLAYERSCRIPT)
    , m_LightDir{}
    , m_Dist(1500.f)
{
}

CFollowPlayerScript::~CFollowPlayerScript()
{
}

void CFollowPlayerScript::begin()
{
    m_LightDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    m_LightDir.Normalize();

    m_Dist = 1500.f; 
}

void CFollowPlayerScript::tick()
{
    // Player가 없다면 리턴
    if (nullptr == PLAYER)
        return;

    Vec3 CurPos = Transform()->GetWorldPos();
    Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();

    CurPos = PlayerPos + (-m_LightDir * m_Dist);

    Transform()->SetWorldPos(CurPos);
}

UINT CFollowPlayerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CFollowPlayerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}