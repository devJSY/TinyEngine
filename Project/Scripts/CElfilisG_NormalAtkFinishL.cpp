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

void CElfilisG_NormalAtkFinishL::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::End:
    {
        ELFFSM->SetComboLevel(0);
    }
        break;
    }
}

void CElfilisG_NormalAtkFinishL::Start()
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
            if (ELFFSM->GetPhase() == 1)
            {
                ELFFSM->RepeatState(L"GROUND_ATK_NORMAL_L");
            }
            else
            {
                ELFFSM->RepeatState(L"GROUND_ATK_NORMAL_R");
            }
        }
        else
        {
            ELFFSM->ChangeStateGroup_RandState(NextState);
        }
    }
}