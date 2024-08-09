#include "pch.h"
#include "CElfilisD_Jump.h"
#include "CElfilisFSM.h"

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

        //@CAMERA 뒤로
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("JumpStart"), false);

        // Jump
        m_StartPos = GetOwner()->Transform()->GetWorldPos();
        m_TargetPos = -GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * ELFFSM->GetAirPos().z + Vec3(0.f, ELFFSM->GetAirPos().y, 0.f);
        Vec3 JumpDir = m_TargetPos - m_StartPos;
        JumpDir.Normalize();

        GetOwner()->Rigidbody()->AddForce(JumpDir * 3000.f, ForceMode::Impulse);

        //@CAMERA 잠깐고정
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

void CElfilisD_Jump::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_APPEAR2_ROAR");
    }
}