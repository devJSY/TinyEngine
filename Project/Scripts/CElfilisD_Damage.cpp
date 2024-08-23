#include "pch.h"
#include "CElfilisD_Damage.h"
#include "CElfilisFSM.h"

#include "CCameraController.h"

CElfilisD_Damage::CElfilisD_Damage()
    : m_PrevDrag(0.f)
{
}

CElfilisD_Damage::~CElfilisD_Damage()
{
}

void CElfilisD_Damage::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Progress: {
        Process();
    }
    break;
    }
}

void CElfilisD_Damage::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Vec3 Dir = PLAYER->Transform()->GetWorldPos();
        Dir.y = 0.f;
        Dir.Normalize();

        GetOwner()->Animator()->Play(ANIMPREFIX("Damage"), false, false, 2.5f, 0.f);
        GetOwner()->Rigidbody()->SetKinematic(true);
        GetOwner()->Transform()->SetWorldPos(Vec3());
        GetOwner()->Transform()->Slerp(Dir, 1.f);

        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();

        // Fixed View
        CAMERACTRL->SetMainTarget(BOSS);

        CAMERACTRL->FixedView(true, Vec3(-132.45f, 83.80f, -140.07f));

        CAMERACTRL->SetOffset(Vec3(0.f, 0.f, 0.f));
        CAMERACTRL->SetMinSpeed(200.f);
        CAMERACTRL->SetMaxSpeed(500.f);
        CAMERACTRL->SetThresholdDistance(500.f);
        CAMERACTRL->SetRotationSpeed(150.f);
        CAMERACTRL->SetZoomMinSpeed(100.f);
        CAMERACTRL->SetZoomMaxSpeed(500.f);
        CAMERACTRL->SetZoomThreshold(500.f);
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 75.f, 0.f));
    }
    break;
    case StateStep::Progress: {
        // BackStep
        Vec3 BackDir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -1.f;
        BackDir.y = 0.f;
        BackDir.Normalize();

        GetOwner()->Rigidbody()->AddForce(BackDir * 700.f, ForceMode::Impulse);
        m_TargetPos = BackDir * 300.f;
    }
    break;
    }
}

void CElfilisD_Damage::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Rigidbody()->SetKinematic(false);
    }
    break;
    case StateStep::Progress:
    {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
        GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
    }
        break;
    }
}

void CElfilisD_Damage::Start()
{
    ChangeStep(StateStep::Progress);
}

void CElfilisD_Damage::Process()
{
    // Add Drag
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
    float t = NewPos.Length() / m_TargetPos.Length();
    float Ratio = clamp(t, 0.f, 1.f) * XM_PI;
    float NewDrag = 3.f - 3.f * sinf(Ratio);
    GetOwner()->Rigidbody()->SetDrag(NewDrag);

    if (t >= 1.f)
    {
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_APPEAR2_JUMP");
    }
}