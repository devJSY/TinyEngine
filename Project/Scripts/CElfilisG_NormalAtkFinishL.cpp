#include "pch.h"
#include "CElfilisG_NormalAtkFinishL.h"
#include "CElfilisFSM.h"

CElfilisG_NormalAtkFinishL::CElfilisG_NormalAtkFinishL()
{
}

CElfilisG_NormalAtkFinishL::~CElfilisG_NormalAtkFinishL()
{
}

void CElfilisG_NormalAtkFinishL::tick()
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
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CElfilisG_NormalAtkFinishL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingLeftStart"), false, false, 1.f, 0.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingFinishLeft"), false, false, 1.f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingFinishLeftEnd"), false, false, 1.f);
    }
    break;
    }
}

void CElfilisG_NormalAtkFinishL::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::End: {
        ELFFSM->ClearComboLevel();
    }
    break;
    }
}

void CElfilisG_NormalAtkFinishL::Start()
{
    // rotate
    RotateToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_NormalAtkFinishL::Progress()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_NormalAtkFinishL::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ELFFSM->GetCurStateGroup())
        {
            ELFFSM->RepeatState(L"GROUND_ATK_NORMAL");
        }
        else
        {
            ELFFSM->ChangeStateGroup(NextState);
        }
    }
}