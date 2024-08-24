#include "pch.h"
#include "CElfilisUnit.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CState.h"

CElfilisUnit::CElfilisUnit()
    : CUnitScript(ELFILISUNIT)
{
    UnitInfo ElfilisInfo = {
        2000.f, // HP
        2000.f, // MaxHP
        10.f,   // Speed
        10.f,   // Rotation Speed
        10.f,   // JumpPower
        0.f,    // ATK
    };
    SetInitInfo(ElfilisInfo);

    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.HP, "[Init] HP");
    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.MAXHP, "[Init] HP max");
    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.Speed, "[Init] Speed");
    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.RotationSpeed, "[Init] Rotation Speed");
    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitInfo.JumpPower, "[Init] Jump Power");

    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "[Current] HP");
    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "[Current] HP max");
    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "[Current] Speed");
    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "[Current] Rotation Speed");
    // AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "[Current] Jump Power");
}

CElfilisUnit::CElfilisUnit(const CElfilisUnit& _Origin)
    : CUnitScript(_Origin)
{
}

CElfilisUnit::~CElfilisUnit()
{
}

void CElfilisUnit::tick()
{
    CUnitScript::tick();

    // Check Phase
    if (ELFFSM->GetPhase() == 1)
    {
        ElfilisStateGroup CurStateGroup = ELFFSM->GetCurStateGroup();

        if ((CurStateGroup >= ElfilisStateGroup::GroundIdle || CurStateGroup <= ElfilisStateGroup::GroundAtkFar) &&
            GetCurInfo().HP <= GetCurInfo().MAXHP * 0.4f)
        {
            ELFFSM->SetPhase(2);
            ELFFSM->ResetFSM();
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_APPEAR2_DAMAGE");
        }
    }
    else
    {
        // Death
        if (GetCurInfo().HP <= 0.f)
        {
        }
        // Resist
        else if (GetCurInfo().HP <= 50.f && !ELFFSM->IsResist())
        {
            ElfilisStateGroup CurStateGroup = ELFFSM->GetCurStateGroup();
            if ((CurStateGroup >= ElfilisStateGroup::GroundIdle || CurStateGroup <= ElfilisStateGroup::GroundAtkFar))
            {
                ELFFSM->ResetFSM();
                ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_RESIST");
            }
        }
    }

    if (KEY_TAP(KEY::SPACE))
    {
        m_CurInfo.HP = 500.f;
    }
    if (KEY_TAP(KEY::ENTER))
    {
        m_CurInfo.HP = 10.f;
    }
}

void CElfilisUnit::ResistSuccess()
{
    m_CurInfo.HP += m_CurInfo.MAXHP * 0.1f;
}

UINT CElfilisUnit::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    UnitInfo ElfilisInfo = {
        2000.f, // HP
        2000.f, // MaxHP
        10.f,   // Speed
        10.f,   // Rotation Speed
        10.f,   // JumpPower
        0.f,    // ATK
    };
    SetInitInfo(ElfilisInfo);
    MemoryByte += CUnitScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CElfilisUnit::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CUnitScript::LoadFromLevelFile(_File);
    UnitInfo ElfilisInfo = {
        2000.f, // HP
        2000.f, // MaxHP
        10.f,   // Speed
        10.f,   // Rotation Speed
        10.f,   // JumpPower
        0.f,    // ATK
    };
    SetInitInfo(ElfilisInfo);
    return MemoryByte;
}