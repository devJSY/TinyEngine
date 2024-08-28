#include "pch.h"
#include "CMorphoDemo_Phase2.h"
#include "CMorphoFSM.h"

#include "CCameraController.h"

CMorphoDemo_Phase2::CMorphoDemo_Phase2()
{
}

CMorphoDemo_Phase2::~CMorphoDemo_Phase2()
{
}

void CMorphoDemo_Phase2::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::StartEnd: {
        StartEnd();
    }
    break;
    case StateStep::Progress: {
        Progress();
    }
    break;
    }
}

void CMorphoDemo_Phase2::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Damage2"), false, false, 1.5f);
        GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
        GetOwner()->Rigidbody()->SetUseGravity(true);

        GetOwner()->Rigidbody()->SetVelocity(Vec3(0.f, 5.f, 0.f));
        MRPFSM->SetGlobalState(true);
    }
    break;
    case StateStep::StartEnd: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DeathLanding"), false, false, 1.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Appeal"), false, false, 1.5f, 0.3f);

        // Camera : ¸ôÆ÷ Å¸°Ù Distortion
        CAMERACTRL->SetMainTarget(BOSS);
        CAMERACTRL->Normal(false);

        Vec3 Dir = BOSS->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Dir.y = 0.f;
        Dir.Normalize();

        CAMERACTRL->SetLookDist(100.f);
        CAMERACTRL->SetLookDir(-Dir);
        CAMERACTRL->SetOffset(Vec3(0.f, 10.f, 0.f));
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 10.f, 0.f));

        CAMERACTRL->SetMinSpeed(500.f);
        CAMERACTRL->SetMaxSpeed(1000.f);
        CAMERACTRL->SetThresholdDistance(500.f);

        CAMERACTRL->SetRotationSpeed(180.f);
        CAMERACTRL->SetZoomMinSpeed(500.f);
        CAMERACTRL->SetZoomMaxSpeed(1000.f);
        CAMERACTRL->SetZoomThreshold(500.f);
    }
    break;
    }
}

void CMorphoDemo_Phase2::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::StartEnd:
        break;
    case StateStep::Progress: {
        // Camera : º¹±¸
        CAMERACTRL->LoadInitSetting(true);
        CAMERACTRL->SetMorphoTwoTarget();
    }
    break;
    }
}

void CMorphoDemo_Phase2::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::StartEnd);
    }
}

void CMorphoDemo_Phase2::StartEnd()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoDemo_Phase2::Progress()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->SetGlobalState(false);
        MRPFSM->SetPattern(MorphoPatternType::Demo_TeleportDoubleSword);
        MRPFSM->ProcPatternStep();
    }
}