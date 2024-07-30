#include "pch.h"
#include "CElfilisG_RayArrow.h"
#include "CElfilisFSM.h"
#include "CElfilisArrowSetScript.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

CElfilisG_RayArrow::CElfilisG_RayArrow()
    : m_ArrowsPref(nullptr)
    , m_Arrows(nullptr)
    , m_ArrowSet(nullptr)
{
    m_ArrowsPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ElfilisArrowSet.pref");
}

CElfilisG_RayArrow::~CElfilisG_RayArrow()
{
}

void CElfilisG_RayArrow::tick()
{
    switch (m_Step)
    {
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

void CElfilisG_RayArrow::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowReady"), false);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowStart"), false);

        if (m_ArrowsPref != nullptr)
        {
            m_Arrows = m_ArrowsPref->Instantiate();
            m_ArrowSet = m_Arrows->GetScript<CElfilisArrowSetScript>();
            if (m_ArrowSet)
            {
                m_ArrowSet->SetTarget(PLAYER);
            }

            GamePlayStatic::SpawnGameObject(m_Arrows, LAYER_MONSTERATK_TRIGGER);
        }
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowWait"));
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowEnd"), false, false, 1.f);
    }
    break;
    }
}

void CElfilisG_RayArrow::Exit_Step()
{
    switch (m_Step)
    {
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

void CElfilisG_RayArrow::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_RayArrow::Progress()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisG_RayArrow::Wait()
{
    if (m_Arrows && m_ArrowSet)
    {
        if (m_ArrowSet->IsSpawnFinish())
        {
            ChangeStep(StateStep::End);
        }
    }
}

void CElfilisG_RayArrow::End()
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
            ELFFSM->ChangeStateGroup_RandState(NextState);
        }
    }
}