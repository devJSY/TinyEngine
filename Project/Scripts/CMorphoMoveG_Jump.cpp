#include "pch.h"
#include "CMorphoMoveG_Jump.h"
#include "CMorphoFSM.h"

CMorphoMoveG_Jump::CMorphoMoveG_Jump()
    : m_StartHeight(0.f)
    , m_JumpHeight(80.f)
    , m_PrevDrag(0.f)
{
}

CMorphoMoveG_Jump::~CMorphoMoveG_Jump()
{
}

void CMorphoMoveG_Jump::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CMorphoMoveG_Jump::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("JumpStart"), false, false, 1.5f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Jump"), false, false, 1.5f);

        // Jump
        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, 25.f, 0.f), ForceMode::Impulse);
        m_StartHeight = GetOwner()->Transform()->GetWorldPos().y;
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
    }
    break;
    }
}

void CMorphoMoveG_Jump::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::End: {
        GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
    }
        break;
    }
}

void CMorphoMoveG_Jump::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoMoveG_Jump::End()
{
    // Add drag
    float NewHeight = GetOwner()->Transform()->GetWorldPos().y;
    float CurDist = NewHeight - m_StartHeight;
    float Ratio = clamp((CurDist / (m_JumpHeight - m_StartHeight)), 0.f, 1.f) * XM_PI;
    float NewDrag = 4.f - 4.f * sinf(Ratio);
    GetOwner()->Rigidbody()->SetDrag(NewDrag);

    if (NewHeight >= m_JumpHeight)
    {
        MRPFSM->ChangeState(L"MOVEG_HOVERDASH");
    }
}