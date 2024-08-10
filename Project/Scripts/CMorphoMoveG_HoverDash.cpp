#include "pch.h"
#include "CMorphoMoveG_HoverDash.h"
#include "CMorphoFSM.h"

CMorphoMoveG_HoverDash::CMorphoMoveG_HoverDash()
    : m_PrevDrag(0.f)
    , m_Speed(130.f)
    , m_NewDrag(0.f)
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

void CMorphoMoveG_HoverDash::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("HoverDashStart"), false, false, 1.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("HoverDash"), true, false, 1.5f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("HoverDashEnd"), false, false, 1.5f);
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        m_NewDrag = 1.f;
    }
    break;
    case StateStep::EndWait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Landing"), false, false, 1.5f);
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
    case StateStep::EndWait:
        break;
    }
}

void CMorphoMoveG_HoverDash::Start()
{
    FlyToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoMoveG_HoverDash::Progress()
{
    FlyToPlayer();

    if (MRPFSM->IsNearPlayer())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoMoveG_HoverDash::End()
{
    // Add drag
    m_NewDrag += 3.f * DT;
    GetOwner()->Rigidbody()->SetDrag(m_NewDrag);

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
    }
}

void CMorphoMoveG_HoverDash::FlyToPlayer()
{
    // move
    Vec3 Veloc = GetOwner()->Rigidbody()->GetVelocity();
    Vec3 Force = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
    Force.y = 0.f;
    Force = Force.Normalize() * m_Speed;

    if (Veloc.Length() <= m_Speed)
    {
        GetOwner()->Rigidbody()->AddForce(Force, ForceMode::Force);
    }
    else
    {
        Force = (Veloc + Force * 1.3f * DT).Normalize() * m_Speed;
        GetOwner()->Rigidbody()->SetVelocity(Force);
    }

    // rotate
    Force.y = 0.f;
    Force.Normalize();
    GetOwner()->Transform()->Slerp(Force, BOSSUNIT->GetInitInfo().RotationSpeed * DT);
}
