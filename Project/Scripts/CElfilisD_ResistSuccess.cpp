#include "pch.h"
#include "CElfilisD_ResistSuccess.h"
#include "CElfilisFSM.h"
#include "CElfilisUnit.h"
#include "CBossLevelFlowMgr.h"

CElfilisD_ResistSuccess::CElfilisD_ResistSuccess()
    : m_bFrmEnter(true)
{
}

CElfilisD_ResistSuccess::~CElfilisD_ResistSuccess()
{
}

void CElfilisD_ResistSuccess::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CElfilisD_ResistSuccess::Exit()
{
    ELFFSM->SetResist(false);
    Exit_Step();
}

void CElfilisD_ResistSuccess::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ResistSuccess"), false, false, 1.5f);
        ELFFSM->SetResist(true);
        ELFFSM->GetUnit()->AddResistHP();
        CBossMgr::GetBossFlowMgr()->BossRevive();

        CBossMgr::GetBossFlowMgr()->ChangeFlow(BossLevelFlow::Fight);

        // Sound
        wstring Roar = L"sound\\wav\\CharaBossChimera2\\0027_Roar.wav";
        GamePlayStatic::Play2DSound(Roar, 1, SOUND_ELFILIS);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Roar"), false, false, 1.5f, 0.3f);
        m_bFrmEnter = true;
    }
    break;
    }
}

void CElfilisD_ResistSuccess::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisD_ResistSuccess::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisD_ResistSuccess::End()
{
    // Sound
    if (CHECK_ANIMFRM(GetOwner(), 140) && m_bFrmEnter)
    {
        m_bFrmEnter = false;

        wstring Recovery = L"sound\\wav\\CharaBossChimera2\\0059_RecoveryEnd.wav";
        GamePlayStatic::Play2DSound(Recovery, 1, SOUND_ELFILIS);
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundMove);
    }
}