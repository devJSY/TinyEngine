#include "pch.h"
#include "CFollowPlayerScript.h"
#include "CPlayerMgr.h"

CFollowPlayerScript::CFollowPlayerScript()
    : CScript(FOLLOWPLAYERSCRIPT)
    , m_bPosOffset(true)
    , m_bRotOffset(true)
    , m_bFollowPos{false,}
    , m_bFollowRot{false,}
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bPosOffset, "Use Offset (Pos)");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bRotOffset, "Use Offset (Rot)");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowPos[0], "Follow Pos X");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowPos[1], "Follow Pos Y");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowPos[2], "Follow Pos Z");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowRot[0], "Follow Rot X");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowRot[1], "Follow Rot Y");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowRot[2], "Follow Rot Z");
}

CFollowPlayerScript::CFollowPlayerScript(const CFollowPlayerScript& _Origin)
    : CScript(FOLLOWPLAYERSCRIPT)
    , m_bPosOffset(_Origin.m_bPosOffset)
    , m_bRotOffset(_Origin.m_bRotOffset)
{
    for (int i = 0; i < 3; ++i)
    {
        m_bFollowPos[i] = _Origin.m_bFollowPos[i];
        m_bFollowRot[i] = _Origin.m_bFollowRot[i];
    }

    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bPosOffset, "Use Offset (Pos)");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bPosOffset, "Use Offset (Rot)");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowPos[0], "Follow Pos X");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowPos[1], "Follow Pos Y");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowPos[2], "Follow Pos Z");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowRot[0], "Follow Rot X");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowRot[1], "Follow Rot Y");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bFollowRot[2], "Follow Rot Z");
}

CFollowPlayerScript::~CFollowPlayerScript()
{
}

void CFollowPlayerScript::begin()
{
    m_PosOffset = Transform()->GetLocalPos();
    m_RotOffset = Transform()->GetLocalRotation();
}

void CFollowPlayerScript::tick()
{
    if (!PLAYER)
        return;

    Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
    Vec3 PlayerRot = PLAYER->Transform()->GetWorldRotation();
    Vec3 NewPos;
    Vec3 NewRot;

    // position
    if (m_bFollowPos[0])
    {
        NewPos.x = m_bPosOffset ? PlayerPos.x + m_PosOffset.x : PlayerPos.x;
    }
    if (m_bFollowPos[1])
    {
        NewPos.y = m_bPosOffset ? PlayerPos.y + m_PosOffset.y : PlayerPos.y;
    }
    if (m_bFollowPos[2])
    {
        NewPos.z = m_bPosOffset ? PlayerPos.z + m_PosOffset.z : PlayerPos.z;
    }

    // rotation
    if (m_bFollowRot[0])
    {
        NewRot.x = m_bRotOffset ? PlayerRot.x + m_PosOffset.x : NewRot.x;
    }
    if (m_bFollowRot[1])
    {
        NewRot.y = m_bRotOffset ? PlayerRot.y + m_PosOffset.y : NewRot.y;
    }
    if (m_bFollowRot[2])
    {
        NewRot.z = m_bRotOffset ? PlayerRot.z + m_PosOffset.z : NewRot.z;
    }

    Transform()->SetWorldPos(NewPos);
    Transform()->SetWorldRotation(NewRot);
}

UINT CFollowPlayerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bPosOffset, sizeof(bool), 1, _File);
    fwrite(&m_bRotOffset, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    for (int i = 0; i < 3; i++)
    {
        fwrite(&m_bFollowPos[i], sizeof(bool), 1, _File);
        MemoryByte += sizeof(bool);
    }
    
    for (int i = 0; i < 3; i++)
    {
        fwrite(&m_bFollowRot[i], sizeof(bool), 1, _File);
        MemoryByte += sizeof(bool);
    }

    return MemoryByte;
}

UINT CFollowPlayerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bPosOffset, sizeof(bool), 1, _File);
    fread(&m_bRotOffset, sizeof(bool), 1, _File);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    for (int i = 0; i < 3; i++)
    {
        fread(&m_bFollowPos[i], sizeof(bool), 1, _File);
        MemoryByte += sizeof(bool);
    }

    for (int i = 0; i < 3; i++)
    {
        fread(&m_bFollowRot[i], sizeof(bool), 1, _File);
        MemoryByte += sizeof(bool);
    }

    return MemoryByte;
}