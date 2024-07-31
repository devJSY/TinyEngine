#include "pch.h"
#include "CElfilisG_GroundToAir.h"
#include "CElfilisFSM.h"

CElfilisG_GroundToAir::CElfilisG_GroundToAir()
    : m_PrevDrag(0.f)
    , m_NewDrag(0.f)
{
}

CElfilisG_GroundToAir::~CElfilisG_GroundToAir()
{
}

void CElfilisG_GroundToAir::tick()
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

void CElfilisG_GroundToAir::Enter_Step()
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
        m_JumpDir = (Vec3(0.f, 1.f, 0.f) - GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT)).Normalize();
        m_JumpDir = m_JumpDir * ELFFSM->GetAirPos() - GetOwner()->Transform()->GetWorldPos();
        m_JumpDir.Normalize();
        GetOwner()->Rigidbody()->AddForce(m_JumpDir * 3500.f, ForceMode::Impulse);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AwayFastEndAir"), false);
    }
    break;
    }
}

void CElfilisG_GroundToAir::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_GroundToAir::Start()
{
    // look player
    RotateToPlayer(DT * 10.f);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_GroundToAir::Progress()
{
    m_NewDrag += DT * 4.f;
    if (m_NewDrag > 3.f)
    {
        m_NewDrag = 3.f;
    }
    GetOwner()->Rigidbody()->SetDrag(m_NewDrag);

    if (GetOwner()->Transform()->GetWorldPos().y >= ELFFSM->GetAirPos().y)
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_GroundToAir::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ElfilisStateGroup::GroundAtkFar || NextState == ElfilisStateGroup::GroundAtkNear)
        {
            ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_SWORDWAVE_RL");
        }
        else
        {
            ELFFSM->ChangeStateGroup_RandState(NextState);
        }
    }
}