#include "pch.h"
#include "CElfilisA_RayArrowUp.h"
#include "CElfilisFSM.h"
#include "CElfilisArrowSetScript.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

CElfilisA_RayArrowUp::CElfilisA_RayArrowUp()
    : m_Arrow{}
    , m_ArrowScript{}
{
    Ptr<CPrefab> ArrowPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisAirArrow.pref", L"prefab\\ElfilisAirArrow.pref");
    
    if (ArrowPref != nullptr)
    {
        for (int i = 0; i < 7; i++)
        {
            m_Arrow[i] = ArrowPref->Instantiate();
            CElfilisArrowSetScript* Script = m_Arrow[i]->GetScript<CElfilisArrowSetScript>();
            
            if (Script)
            {
                m_ArrowScript[i] = Script;
            }
        }
    }
}

CElfilisA_RayArrowUp::~CElfilisA_RayArrowUp()
{
    for (int i = 0; i < 7; ++i)
    {
        if (m_Arrow[i] && m_Arrow[i]->GetLayerIdx() != -1)
        {
            delete m_Arrow[i];
        }
    }
}

void CElfilisA_RayArrowUp::tick()
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

void CElfilisA_RayArrowUp::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowReadyAir"), false);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowStartAir"), false);

        //if (m_ArrowsPref != nullptr)
        //{
        //    m_Arrow = m_ArrowsPref->Instantiate();
        //    m_ArrowScript = m_Arrow->GetScript<CElfilisArrowSetScript>();
        //    if (m_ArrowScript)
        //    {
        //        m_ArrowScript->SetTarget(PLAYER);
        //    }

        //    GamePlayStatic::SpawnGameObject(m_Arrow, LAYER_MONSTERATK_TRIGGER);
        //}
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowWaitAir"));
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowEndAir"), false);
    }
    break;
    }
}

void CElfilisA_RayArrowUp::Exit_Step()
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

void CElfilisA_RayArrowUp::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisA_RayArrowUp::Progress()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisA_RayArrowUp::Wait()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisA_RayArrowUp::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ELFFSM->GetCurStateGroup())
        {
            //ELFFSM->RepeatState(L"AIR_ATKS_RAYARROW_DOWN");
        }
        else
        {
            ELFFSM->ChangeStateGroup_RandState(NextState);
        }
    }
}