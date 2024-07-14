#include "pch.h"
#include "CKirbyUnitScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

CKirbyUnitScript::CKirbyUnitScript()
    : CUnitScript(KIRBYUNITSCRIPT)
{
    UnitInfo KirbyInfo = {
        100.f, // HP
        10.f,  // Speed
        10.f,  // JumpPower
        100.f, // MaxHP
    };
    SetInfo(KirbyInfo);

    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
}

CKirbyUnitScript::CKirbyUnitScript(const CKirbyUnitScript& _Origin)
    : CUnitScript(_Origin)
{
    UnitInfo KirbyInfo = {
        100.f, // HP
        10.f,  // Speed
        10.f,  // JumpPower
        100.f, // MaxHP
    };
    SetInfo(KirbyInfo);

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
}

void CKirbyUnitScript::tick()
{
    CUnitScript::tick();
}

void CKirbyUnitScript::AttackReward()
{
    if (PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::SWORD)
    {
        m_CurInfo.HP += 2.5f;

        if (m_CurInfo.HP > m_CurInfo.MAXHP)
        {
            m_CurInfo.HP = m_CurInfo.MAXHP;
        }
    }
}

void CKirbyUnitScript::SaveToLevelFile(FILE* _File)
{
    CUnitScript::SaveToLevelFile(_File);
}

void CKirbyUnitScript::LoadFromLevelFile(FILE* _File)
{
    CUnitScript::LoadFromLevelFile(_File);

    if (m_CurInfo.MAXHP == 0.f)
    {
        UnitInfo KirbyInfo = {
            100.f, // HP
            10.f,  // Speed
            10.f,  // JumpPower
            100.f, // MaxHP
        };

        SetInfo(KirbyInfo);
    }
}
