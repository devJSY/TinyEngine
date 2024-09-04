#include "pch.h"
#include "CElfilisG_GroundToAir.h"
#include "CElfilisFSM.h"

#include "CCameraController.h"

CElfilisG_GroundToAir::CElfilisG_GroundToAir()
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

        // Camera : ÇÏ´Ã ºä
        CAMERACTRL->SetElfilisSky();
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AwayFastStart"), false);

        // Jump
        m_StartPos = GetOwner()->Transform()->GetWorldPos();
        m_TargetPos = -GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * ELFFSM->GetAirPos().z + Vec3(0.f, ELFFSM->GetAirPos().y, 0.f);
        m_ForceDir = m_TargetPos - m_StartPos;
        m_ForceDir.Normalize();

        // sound
        wstring Start = L"sound\\wav\\CharaBossChimera2\\0025_MoveAir.wav";
        GamePlayStatic::Play2DSound(Start, 1, SOUND_ELFILIS * 0.6f);
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
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
    float CurDist = (NewPos - m_StartPos).Length();
    float Ratio = CurDist / (m_TargetPos - m_StartPos).Length();

    // move
    float EndRatio = 0.3f;

    if (Ratio < EndRatio)
    {
        Ratio = (Ratio / EndRatio) * 0.98f + 0.01f;
        Ratio = clamp(sinf(Ratio * XM_PI / 2.f) * 2.f, 0.f, 1.f);
        float NewSpeed = 100.f * Ratio;
        GetOwner()->Rigidbody()->SetVelocity(m_ForceDir * NewSpeed);
    }

    // move end
    else
    {
        Ratio = (Ratio - EndRatio) / (1.f - EndRatio);
        Ratio = Ratio * 0.8f + 0.1f;
        Ratio = clamp(cosf(Ratio * XM_PI / 2.f), 0.f, 1.f);
        float NewSpeed = 100.f * Ratio;
        GetOwner()->Rigidbody()->SetVelocity(m_ForceDir * NewSpeed);
    }

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