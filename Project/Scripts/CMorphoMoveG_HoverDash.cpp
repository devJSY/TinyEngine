#include "pch.h"
#include "CMorphoMoveG_HoverDash.h"
#include "CMorphoFSM.h"

CMorphoMoveG_HoverDash::CMorphoMoveG_HoverDash()
    : m_Speed(20.f)
    , m_RotSpeed(3.f)
    , m_PrevDrag(0.f)
    , m_AccTime(0.f)
{
}

CMorphoMoveG_HoverDash::~CMorphoMoveG_HoverDash()
{
}

void CMorphoMoveG_HoverDash::tick()
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
    case StateStep::EndWait: {
        EndWait();
    }
    break;
    }
}

void CMorphoMoveG_HoverDash::Exit()
{
    Exit_Step();

    GetOwner()->Rigidbody()->SetVelocity(Vec3());
    GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
}

void CMorphoMoveG_HoverDash::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("HoverDashStart"), false, false, 1.5f);
        m_AccTime = 0.f;
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("HoverDash"), true, false, 1.5f);

        // sound
        wstring Sound = L"sound\\wav\\CharaMisterFrosty\\0003_Fly.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("HoverDashEnd"), false, false, 1.5f);
    }
    break;
    case StateStep::EndWait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Landing"), false, false, 1.5f);
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        GetOwner()->Rigidbody()->SetDrag(2.f);
    }
    break;
    }
}

void CMorphoMoveG_HoverDash::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::End:
        break;
    case StateStep::EndWait: {
        GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
    }
    break;
    }
}

void CMorphoMoveG_HoverDash::Start()
{
    m_AccTime += DT;
    FlyToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoMoveG_HoverDash::Progress()
{
    m_AccTime += DT;
    FlyToPlayer();

    if (MRPFSM->IsNearPlayer())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoMoveG_HoverDash::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::EndWait);
    }
}

void CMorphoMoveG_HoverDash::EndWait()
{
    if (MRPFSM->IsGround())
    {
        MRPFSM->Attack();

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\0001_Landing.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 1.3f);
    }
}

void CMorphoMoveG_HoverDash::FlyToPlayer()
{
    float AppearTime = 2.f;

    // move
    Vec3 Dir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
    Dir.y = 0.f;
    float Dist = Dir.Length();
    Dir.Normalize();

    Vec3 Force = Dir * m_Speed;
    Force = (GetOwner()->Rigidbody()->GetVelocity() + Force * m_RotSpeed * DT).Normalize() * m_Speed;

    // chase 0 : start
    if (m_AccTime <= AppearTime)
    {
        float t = m_AccTime / AppearTime;
        float Delta = sinf(XM_PI / 2.f * t);
        GetOwner()->Rigidbody()->SetVelocity(Force * Delta);
    }

    // chase 1 : end
    else if (Dist <= MRPFSM->GetNearDist() + 10.f)
    {
        float t = Dist - MRPFSM->GetNearDist() / 10.f;
        float Delta = cosf(XM_PI / 2.f * t);
        Delta = Delta * 0.9f + 0.1f; // 0.1 ~ 1.0
        GetOwner()->Rigidbody()->SetVelocity(Force * Delta);
    }

    // chase 2 : ing
    else
    {
        GetOwner()->Rigidbody()->SetVelocity(Force);
    }

    // rotate
    Force.y = 0.f;
    Force.Normalize();
    GetOwner()->Transform()->Slerp(Force, m_RotSpeed * DT);
}
