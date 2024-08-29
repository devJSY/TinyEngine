#include "pch.h"
#include "CElfilisUnit.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CState.h"

CElfilisUnit::CElfilisUnit()
    : CUnitScript(ELFILISUNIT)
{
    UnitInfo ElfilisInfo = {
        1000.f, // HP
        1000.f, // MaxHP
        10.f,   // Speed
        10.f,   // Rotation Speed
        10.f,   // JumpPower
        0.f,    // ATK
    };

    SetInitInfo(ElfilisInfo);
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

    // Death & Resist
    if (m_CurInfo.HP <= m_InitInfo.MAXHP * 0.05f && !ELFFSM->IsResist())
    {
        ElfilisStateGroup CurStateGroup = ELFFSM->GetCurStateGroup();
        if ((CurStateGroup >= ElfilisStateGroup::GroundIdle || CurStateGroup <= ElfilisStateGroup::GroundAtkFar))
        {
            ELFFSM->ResetFSM();
            m_CurInfo.HP = m_InitInfo.MAXHP * 0.05f; 
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_RESIST");
        }
    }

    // Phase 1
    else if (ELFFSM->GetPhase() == 1)
    {
        // Cheat : Phase 1 -> Phase 2
        if ((KEY_PRESSED(KEY::LCTRL) && (KEY_TAP(KEY::ENTER))) || (KEY_TAP(KEY::LCTRL) && (KEY_PRESSED(KEY::ENTER))))
        {
            m_CurInfo.HP = m_InitInfo.HP * 0.4f;
        }
        
        ElfilisStateGroup CurStateGroup = ELFFSM->GetCurStateGroup();

        if ((CurStateGroup >= ElfilisStateGroup::GroundIdle || CurStateGroup <= ElfilisStateGroup::GroundAtkFar) &&
            GetCurInfo().HP <= GetCurInfo().MAXHP * 0.4f)
        {
            ELFFSM->ResetFSM();
            ELFFSM->SetPhase(2);
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_APPEAR2_DAMAGE");
        }
    }

    // Phase 2
    else if (ELFFSM->GetPhase() == 2)
    {
        // Cheat : Death
        if ((KEY_PRESSED(KEY::LCTRL) && (KEY_TAP(KEY::ENTER))) || (KEY_TAP(KEY::LCTRL) && (KEY_PRESSED(KEY::ENTER))))
        {
            m_CurInfo.HP = 0.f;
        }
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