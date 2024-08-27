#include "pch.h"
#include "CElfilisA_MoveR.h"
#include "CElfilisFSM.h"

CElfilisA_MoveR::CElfilisA_MoveR()
    : m_PrevDrag(0.f)
    , m_bMoveRight(false)
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

void CElfilisA_MoveR::Exit()
{
    Exit_Step();

    GetOwner()->Rigidbody()->SetVelocity(Vec3());
    GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
    GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
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
        m_bMoveRight = false;
        m_bMoveUp = false;

        // move down
        Vec3 DownForce = Vec3(0.f, -1.f, 0.f) * 200.f;
        GetOwner()->Rigidbody()->SetDrag(2.f);
        GetOwner()->Rigidbody()->AddForce(DownForce, ForceMode::Impulse);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TurnRightAir"), true);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TurnRightAirEnd"), false);

        // move down
        GetOwner()->Rigidbody()->SetDrag(4.f);
        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, -1.f, 0.f) * 500.f, ForceMode::Impulse);
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
    case StateStep::Progress:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisA_MoveR::Start()
{
    // look player
    RotateToPlayer(DT * 10.f);

    // move
    if (CHECK_ANIMFRM(GetOwner(), 30))
    {
        Fly();
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

    // move
    Fly();
    
    // Change State
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
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
        ELFFSM->ChangeStateGroup(NextState);
    }
}

void CElfilisA_MoveR::Fly()
{
    Vec3 ForceDir = (m_TargetPos - m_StartPos).Normalize();
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
    float CurDist = (NewPos - m_StartPos).Length();
    float Ratio = clamp((CurDist / (m_TargetPos - m_StartPos).Length()), 0.f, 1.f);

    // move Right
    if (!m_bMoveRight)
    {
        m_bMoveRight = true;

        Vec3 RightFrorce = ForceDir * 1700.f;
        RightFrorce += Vec3(0.f, 1.f, 0.f) * 300.f;
        GetOwner()->Rigidbody()->SetDrag(1.6f);
        GetOwner()->Rigidbody()->AddForce(RightFrorce, ForceMode::Impulse);
    }

    // move up
    if (Ratio >= 0.3f && !m_bMoveUp)
    {
        m_bMoveUp = true;

        Vec3 UpForce = Vec3(0.f, 1.f, 0.f) * 300.f;
        GetOwner()->Rigidbody()->AddForce(UpForce, ForceMode::Impulse);
    }

    // finish move
    if ((NewPos - m_StartPos).Length() >= (m_TargetPos - m_StartPos).Length())
    {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
    }
}
