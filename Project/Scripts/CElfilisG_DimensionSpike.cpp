#include "pch.h"
#include "CElfilisG_DimensionSpike.h"
#include "CElfilisFSM.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

CElfilisG_DimensionSpike::CElfilisG_DimensionSpike()
    : m_AccTime(0.f)
    , m_bFrmEnter(true)
{
    m_DimensionSetPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisDimensionSet.pref");
}

CElfilisG_DimensionSpike::~CElfilisG_DimensionSpike()
{
}

void CElfilisG_DimensionSpike::tick()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        Ready();
    }
    break;
    case StateStep::ReadyWait: {
        ReadyWait();
    }
    break;
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Progress: {
        Progress();
    }
    break;
    case StateStep::Wait: {
        Wait();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CElfilisG_DimensionSpike::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpikeReady"), false);
    }
    break;
    case StateStep::ReadyWait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpikeReadyWait"), false);
    }
    break;
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpikeStart"), false);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpike"), false);
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpikeWait"));
        m_AccTime = 0.f;
        m_bFrmEnter = true;
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpikeEnd"), false);
    }
    break;
    }
}

void CElfilisG_DimensionSpike::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Ready:
        break;
    case StateStep::ReadyWait:
        break;
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::Wait:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_DimensionSpike::Ready()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::ReadyWait);
    }
}

void CElfilisG_DimensionSpike::ReadyWait()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Start);
    }
}

void CElfilisG_DimensionSpike::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_DimensionSpike::Progress()
{
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 70))
    {
        if (m_DimensionSetPref != nullptr)
        {
            // strom ¼ÒÈ¯
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisG_DimensionSpike::Wait()
{
    m_AccTime += DT;

    if (m_AccTime >= 3.f)
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_DimensionSpike::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ELFFSM->GetCurStateGroup())
        {
            ELFFSM->RepeatState();
        }
        else
        {
            ELFFSM->ChangeStateGroup(NextState);
        }
    }
}