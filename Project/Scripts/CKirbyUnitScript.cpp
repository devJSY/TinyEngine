#include "pch.h"
#include "CKirbyUnitScript.h"

CKirbyUnitScript::CKirbyUnitScript()
    : CUnitScript(KIRBYUNITSCRIPT)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
}

CKirbyUnitScript::CKirbyUnitScript(const CKirbyUnitScript& _Origin)
    : CUnitScript(_Origin)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
}

CKirbyUnitScript::~CKirbyUnitScript()
{
}

void CKirbyUnitScript::begin()
{
    UnitInfo KirbyInfo = {
        100.f, // HP
        10.f,  // Speed
        10.f,  // JumpPower
        100.f, // MaxHP
    };

    SetInfo(KirbyInfo);
}

void CKirbyUnitScript::SaveToLevelFile(FILE* _File)
{
    CUnitScript::SaveToLevelFile(_File);
}

void CKirbyUnitScript::LoadFromLevelFile(FILE* _File)
{
    CUnitScript::LoadFromLevelFile(_File);
}
