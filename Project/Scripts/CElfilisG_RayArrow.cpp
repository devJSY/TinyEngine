#include "pch.h"
#include "CElfilisG_RayArrow.h"
#include "CElfilisFSM.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

CElfilisG_RayArrow::CElfilisG_RayArrow()
    : m_ArrowsPref(nullptr)
    , m_Arrows(nullptr)
{
    m_ArrowsPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ElfilisArrowSet.prefab");
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
            GamePlayStatic::SpawnGameObject(m_Arrows, LAYER_MONSTERATK_TRIGGER);
            //@TODO 생성 알림
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
    if (m_Arrows)
    {
        //@TODO 커비추적
        //@TODO 돌아가게
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisG_RayArrow::Wait()
{
    if (m_Arrows)
    {
        //@TODO 커비추적
        //@TODO 돌아가게

        //@TODO 만약 생성이 끝났다면 값 바꿈
        //if (생성끝)
        //{
        //    // 공격알림
        //    ChangeStep(StateStep::End);
        //}
    }

    static float test = 0.f;
    test += DT;

    if (test > 2.f)
    {
        ChangeStep(StateStep::End);
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