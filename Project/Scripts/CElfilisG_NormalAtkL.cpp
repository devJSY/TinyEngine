#include "pch.h"
#include "CElfilisG_NormalAtkL.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"

#include "CPlayerMgr.h"

CElfilisG_NormalAtkL::CElfilisG_NormalAtkL()
    : m_Step(StateStep::Start)
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

void CElfilisG_NormalAtkL::Enter()
{
    m_Step = StateStep::Start;
    Enter_Step();
}

void CElfilisG_NormalAtkL::Exit()
{
    Exit_Step();
}

void CElfilisG_NormalAtkL::ChangeStep(StateStep _Step)
{
    Exit_Step();
    m_Step = _Step;
    Enter_Step();
}

void CElfilisG_NormalAtkL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        PLAYER->Animator()->Play(ANIMPREFIX("SwingLeftStart", false));
    }
    break;
    case StateStep::Progress: {
        PLAYER->Animator()->Play(ANIMPREFIX("SwingLeft", false));
    }
    break;
    case StateStep::End: {
        PLAYER->Animator()->Play(ANIMPREFIX("SwingLeftEnd"), false, false, 1.f);
    }
    break;
    }
}

void CElfilisG_NormalAtkL::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        // È¸Àü ¸ØÃã
    }
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
    Dir.Normalize();
    GetOwner()->Transform()->Slerp(Dir, 1.f); 

    if (BOSS->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_NormalAtkL::Progress()
{
    if (BOSS->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_NormalAtkL::End()
{
    if (BOSS->Animator()->IsFinish())
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