#include "pch.h"
#include "CElfilisA_MoveR.h"
#include "CElfilisFSM.h"

CElfilisA_MoveR::CElfilisA_MoveR()
    : m_PrevDrag(0.f)
    , m_bFrmEnter(true)
    , m_bMoveUp(false)
{
}

CElfilisA_MoveR::~CElfilisA_MoveR()
{
}

void CElfilisA_MoveR::tick()
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

void CElfilisA_MoveR::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TurnRightAirStart"), false);

        m_StartPos = GetOwner()->Transform()->GetWorldPos();
        m_TargetPos = XMVector3TransformCoord(m_StartPos, XMMatrixRotationY(XMConvertToRadians(-30.f)));
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        GetOwner()->Rigidbody()->SetDrag(0.f);

        m_bFrmEnter = true;
        m_bMoveUp = false;
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TurnRightAir"), true);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TurnRightAirEnd"), false);
        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, -1.f, 0.f) * 30.f, ForceMode::Impulse);
    }
    break;
    }
}

void CElfilisA_MoveR::Exit_Step()
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
    case StateStep::End: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
    }
    break;
    }
}

void CElfilisA_MoveR::Start()
{
    // look player
    RotateToPlayer(DT * 10.f);

    if (CHECK_ANIMFRM(GetOwner(), 30))
    {
        if (m_bFrmEnter)
        {
            // Start Move
            m_StartPos = GetOwner()->Transform()->GetWorldPos();
            Vec3 Force = (m_TargetPos - m_StartPos).Normalize() * 1500.f;
            GetOwner()->Rigidbody()->SetVelocity(Vec3());
            GetOwner()->Rigidbody()->AddForce(Force, ForceMode::Impulse);
            m_bFrmEnter = false;
        }
        else
        {
            // Add drag
            Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
            float CurDist = (NewPos - m_StartPos).Length();
            float Ratio = clamp((CurDist / (m_TargetPos - m_StartPos).Length()), 0.f, 1.f);
            float NewDrag = 3.f - 3.f * sinf(Ratio * XM_PI);
            GetOwner()->Rigidbody()->SetDrag(NewDrag);

            if (!m_bMoveUp && Ratio >= 0.8f)
            {
                GetOwner()->Rigidbody()->AddForce(Vec3(0.f, 1.f, 0.f) * 150.f, ForceMode::Impulse);
                m_bMoveUp = true;
            }

            if ((NewPos - m_StartPos).Length() >= (m_TargetPos - m_StartPos).Length())
            {
                GetOwner()->Rigidbody()->SetVelocity(Vec3());
            }
        }
    }
    else if (CHECK_ANIMFRM(GetOwner(), 0))
    {
        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, -1.f, 0.f) * 700.f, ForceMode::Force);
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisA_MoveR::Progress()
{
    // look player
    RotateToPlayer();

    // Add drag
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
    float CurDist = (NewPos - m_StartPos).Length();
    float Ratio = clamp((CurDist / (m_TargetPos - m_StartPos).Length()), 0.f, 1.f) * XM_PI;
    float NewDrag = 3.f - 3.f * sinf(Ratio);
    GetOwner()->Rigidbody()->SetDrag(NewDrag);

    if (!m_bMoveUp && Ratio >= 0.8f)
    {
        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, 1.f, 0.f) * 150.f, ForceMode::Impulse);
        m_bMoveUp = true;
    }

    if ((NewPos - m_StartPos).Length() >= (m_TargetPos - m_StartPos).Length())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisA_MoveR::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup_RandState(NextState);
    }
}