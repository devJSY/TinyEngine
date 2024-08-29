#include "pch.h"
#include "CElfilisD_Jump.h"
#include "CElfilisFSM.h"

#include "CCameraController.h"

CElfilisD_Jump::CElfilisD_Jump()
    : m_PrevDrag(0.f)
{
}

CElfilisD_Jump::~CElfilisD_Jump()
{
}

void CElfilisD_Jump::tick()
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

void CElfilisD_Jump::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("JumpReady"), false, false, 2.5f, 0.3f);
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();

        ELFFSM->SetGlobalState(true);

        // Camera : 뒤로
        CAMERACTRL->FixedView(false, Vec3(-78.53f, 131.92f, -91.44f));

    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("JumpStart"), false);

        // Jump
        m_StartPos = GetOwner()->Transform()->GetWorldPos();
        m_TargetPos = -GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * ELFFSM->GetAirPos().z + Vec3(0.f, ELFFSM->GetAirPos().y, 0.f);
        Vec3 JumpDir = m_TargetPos - m_StartPos;
        JumpDir.Normalize();

        GetOwner()->Rigidbody()->SetDrag(3.f);
        GetOwner()->Rigidbody()->SetVelocity(Vec3::Zero);
        GetOwner()->Rigidbody()->AddForce(JumpDir * 3800.f, ForceMode::Impulse);

        // Camera : 잠깐고정
        CAMERACTRL->Normal(false);

        CAMERACTRL->SetLookDir(-BOSS->Transform()->GetWorldDir(DIR_TYPE::FRONT));
        CAMERACTRL->SetLookDist(300.f);

        CAMERACTRL->SetOffset(Vec3(0.f, 0.f, 0.f));
        CAMERACTRL->SetMinSpeed(200.f);
        CAMERACTRL->SetMaxSpeed(1500.f);
        CAMERACTRL->SetThresholdDistance(500.f);
        CAMERACTRL->SetRotationSpeed(100.f);
        CAMERACTRL->SetZoomMinSpeed(100.f);
        CAMERACTRL->SetZoomMaxSpeed(1000.f);
        CAMERACTRL->SetZoomThreshold(500.f);
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 75.f, 0.f));

        CAMERACTRL->SetLock(true,0.5f);

    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("JumpEnd"), false);
    }
    break;
    }
}

void CElfilisD_Jump::Exit_Step()
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

void CElfilisD_Jump::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisD_Jump::Progress()
{
    // Change Step
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
    
    if ((NewPos - m_StartPos).Length() >= (m_TargetPos - m_StartPos).Length())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisD_Jump::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ELFFSM->SetGlobalState(false);
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_APPEAR2_ROAR");
    }
}