#include "pch.h"
#include "CElfilisG_BackStep.h"
#include "CElfilisFSM.h"

CElfilisG_BackStep::CElfilisG_BackStep()
    : m_PrevDrag(0.f)
    , m_NewDrag(0.f)
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
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        m_NewDrag = 0.f;
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
    RotateToPlayer(DT * 10.f);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_BackStep::Progress()
{
    m_NewDrag += DT * 10.f;
    GetOwner()->Rigidbody()->SetDrag(m_NewDrag);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_BackStep::End()
{
    m_NewDrag += DT * 10.f;
    GetOwner()->Rigidbody()->SetDrag(m_NewDrag);

    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ElfilisStateGroup::GroundAtk)
        {
            ELFFSM->ChangeStateGroup_SetState(NextState, L"GROUND_ATK_SWORDWAVE_RL");
            //float Rand = GetRandomfloat(1.f, 100.f);

            //if (Rand <= 45.f)
            //{
            //    ELFFSM->ChangeStateGroup_SetState(NextState, L"GROUND_ATK_SWORDWAVE_RL");
            //}
            //else
            //{
            //    ELFFSM->ChangeStateGroup_SetState(NextState, L"GROUND_ATK_DIMENSIONSPIKE");
            //}
        }
        else
        {
            ELFFSM->ChangeStateGroup_RandState(NextState);
        }
    }
}