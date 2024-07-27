#include "pch.h"
#include "CElfilisG_NormalAtkL.h"
#include "CElfilisFSM.h"

CElfilisG_NormalAtkL::CElfilisG_NormalAtkL()
{
}

CElfilisG_NormalAtkL::~CElfilisG_NormalAtkL()
{
}

void CElfilisG_NormalAtkL::tick()
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

void CElfilisG_NormalAtkL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingLeftStart"), false);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingLeft"), false);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingLeftEnd"), false, false, 1.f);
    }
    break;
    }
}

void CElfilisG_NormalAtkL::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
    break;
    case StateStep::Progress:
    break;
    case StateStep::End:
    break;
    }
}

void CElfilisG_NormalAtkL::Start()
{
    // rotate
    Vec3 Dir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
    Dir.y = 0.f;
    Dir.Normalize();
    GetOwner()->Transform()->Slerp(Dir, 1.f); 

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_NormalAtkL::Progress()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_NormalAtkL::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        //if (NextState == ELFFSM->GetCurStateGroup())
        //{
        //    ELFFSM->RepeatState();
        //}
        //else
        //{
        //    ELFFSM->ChangeStateGroup_RandState(NextState);
        //}
    }
}