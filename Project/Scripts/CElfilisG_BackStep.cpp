#include "pch.h"
#include "CElfilisG_BackStep.h"
#include "CElfilisFSM.h"

CElfilisG_BackStep::CElfilisG_BackStep()
    : m_PrevDrag(0.f)
{
}

CElfilisG_BackStep::~CElfilisG_BackStep()
{
}

void CElfilisG_BackStep::tick()
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

void CElfilisG_BackStep::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AwayFastReady"), false);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AwayFastStart"), false);

        // Jump
        Vec3 JumpDir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -1.f;
        GetOwner()->Rigidbody()->AddForce(JumpDir * 1000.f, ForceMode::Impulse);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AwayFastEnd"), false);
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
    }
    break;
    }
}

void CElfilisG_BackStep::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::End: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
    }
    break;
    }
}

void CElfilisG_BackStep::Start()
{
    // look player
    Vec3 Dir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
    Dir.y = 0.f;
    Dir.Normalize();
    GetOwner()->Transform()->Slerp(Dir, 1.f);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_BackStep::Progress()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_BackStep::End()
{
    // Jump
    float NewDrag = GetOwner()->Rigidbody()->GetDrag() + DT;
    GetOwner()->Rigidbody()->SetDrag(NewDrag);

    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup_RandState(NextState);
    }
}