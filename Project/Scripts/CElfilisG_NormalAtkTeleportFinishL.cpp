#include "pch.h"
#include "CElfilisG_NormalAtkTeleportFinishL.h"
#include "CElfilisFSM.h"

CElfilisG_NormalAtkTeleportFinishL::CElfilisG_NormalAtkTeleportFinishL()
{
}

CElfilisG_NormalAtkTeleportFinishL::~CElfilisG_NormalAtkTeleportFinishL()
{
}

void CElfilisG_NormalAtkTeleportFinishL::tick()
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

void CElfilisG_NormalAtkTeleportFinishL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingLeftStart"), false, false, 2.5f, 0.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingFinishLeft"), false);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingFinishLeftEnd"), false, false, 1.5f);
    }
    break;
    }
}

void CElfilisG_NormalAtkTeleportFinishL::Exit_Step()
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

void CElfilisG_NormalAtkTeleportFinishL::Start()
{
    // rotate
    RotateToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_NormalAtkTeleportFinishL::Progress()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_NormalAtkTeleportFinishL::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup(NextState);
    }
}