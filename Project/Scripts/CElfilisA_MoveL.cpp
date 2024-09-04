#include "pch.h"
#include "CElfilisA_MoveL.h"
#include "CElfilisFSM.h"

CElfilisA_MoveL::CElfilisA_MoveL()
    : m_PrevDrag(0.f)
    , m_bMoveLeft(false)
    , m_bMoveUp(false)
{
    m_SoundKeyMoveAir.push_back(L"sound\\wav\\CharaBossChimera2\\0024_MoveAir.wav");
    m_SoundKeyMoveAir.push_back(L"sound\\wav\\CharaBossChimera2\\0026_MoveAir.wav");
}

CElfilisA_MoveL::~CElfilisA_MoveL()
{
}

void CElfilisA_MoveL::tick()
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

void CElfilisA_MoveL::Exit()
{
    Exit_Step();

    GetOwner()->Rigidbody()->SetVelocity(Vec3());
    GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
    GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
}

void CElfilisA_MoveL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TurnLeftAirStart"), false);

        m_StartPos = GetOwner()->Transform()->GetWorldPos();
        m_TargetPos = XMVector3TransformCoord(m_StartPos, XMMatrixRotationY(XMConvertToRadians(30.f)));
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        m_bMoveLeft = false;
        m_bMoveUp = false;

        // move down
        Vec3 DownForce = Vec3(0.f, -1.f, 0.f) * 200.f;
        GetOwner()->Rigidbody()->SetDrag(2.f);
        GetOwner()->Rigidbody()->SetVelocity(Vec3::Zero);
        GetOwner()->Rigidbody()->AddForce(DownForce, ForceMode::Impulse);

        // sound
        int RandIdx = GetRandomInt(0, 1);
        GamePlayStatic::Play2DSound(m_SoundKeyMoveAir[RandIdx], 1, SOUND_ELFILIS * 0.8f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TurnLeftAir"), true);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TurnLeftAirEnd"), false);

        // move down
        GetOwner()->Rigidbody()->SetDrag(4.f);
        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, -1.f, 0.f) * 500.f, ForceMode::Impulse);
    }
    break;
    }
}

void CElfilisA_MoveL::Exit_Step()
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

void CElfilisA_MoveL::Start()
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

void CElfilisA_MoveL::Progress()
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

void CElfilisA_MoveL::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup(NextState);
    }
}

void CElfilisA_MoveL::Fly()
{
    Vec3 ForceDir = (m_TargetPos - m_StartPos).Normalize();
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
    float CurDist = (NewPos - m_StartPos).Length();
    float Ratio = clamp((CurDist / (m_TargetPos - m_StartPos).Length()), 0.f, 1.f);

    // move left
    if (!m_bMoveLeft)
    {
        m_bMoveLeft = true;

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