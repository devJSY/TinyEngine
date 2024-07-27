#include "pch.h"
#include "CElfilisUnit.h"

CElfilisUnit::CElfilisUnit()
    : CUnitScript(ELFILISUNIT)
{
    UnitInfo KirbyInfo = {
        2000.f, // HP
        2000.f, // MaxHP
        10.f,  // Speed
        10.f,  // Rotation Speed
        10.f,  // JumpPower
        0.f,   // ATK
    };
    SetInitInfo(KirbyInfo);

    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.HP, "[Init] HP");
    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.MAXHP, "[Init] HP max");
    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.Speed, "[Init] Speed");
    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.RotationSpeed, "[Init] Rotation Speed");
    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.JumpPower, "[Init] Jump Power");

    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "[Current] HP");
    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "[Current] HP max");
    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "[Current] Speed");
    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "[Current] Rotation Speed");
    //AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "[Current] Jump Power");
}

CElfilisUnit::CElfilisUnit(const CElfilisUnit& _Origin)
    : CUnitScript(_Origin)
{
}

CElfilisUnit::~CElfilisUnit()
{
}

UINT CElfilisUnit::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CElfilisUnit::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::LoadFromLevelFile(_File);

    return MemoryByte;
}