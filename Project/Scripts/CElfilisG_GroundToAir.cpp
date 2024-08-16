#include "pch.h"
#include "CElfilisG_GroundToAir.h"
#include "CElfilisFSM.h"

#include "CCameraController.h"

CElfilisG_GroundToAir::CElfilisG_GroundToAir()
    : m_PrevDrag(0.f)
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

        // ÇÏ´Ã ºä
        CAMERACTRL->SetElfilisSky();
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AwayFastStart"), false);

        // Jump
        m_StartPos = GetOwner()->Transform()->GetWorldPos();
        m_TargetPos = -GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * ELFFSM->GetAirPos().z + Vec3(0.f, ELFFSM->GetAirPos().y, 0.f);
        Vec3 JumpDir = m_TargetPos - m_StartPos;
        JumpDir.Normalize();

        GetOwner()->Rigidbody()->AddForce(JumpDir * 3000.f, ForceMode::Impulse);
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
    // Add drag
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
    float CurDist = (NewPos - m_StartPos).Length();
    float Ratio = clamp((CurDist / (m_TargetPos - m_StartPos).Length()), 0.f, 1.f) * XM_PI;
    float NewDrag = 4.f - 4.f * sinf(Ratio);
    GetOwner()->Rigidbody()->SetDrag(NewDrag);

    // Change Step
    if ((NewPos - m_StartPos).Length() >= (m_TargetPos - m_StartPos).Length())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_GroundToAir::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup(NextState);
    }
}