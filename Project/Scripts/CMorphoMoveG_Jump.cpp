#include "pch.h"
#include "CMorphoMoveG_Jump.h"
#include "CMorphoFSM.h"

CMorphoMoveG_Jump::CMorphoMoveG_Jump()
    : m_StartHeight(0.f)
    , m_JumpHeight(80.f)
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
        GetOwner()->Rigidbody()->SetVelocity(Vec3::Zero);
        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, 25.f, 0.f), ForceMode::Impulse);
        m_StartHeight = GetOwner()->Transform()->GetWorldPos().y;

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\0002_Jump.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO);
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
    float NewHeight = GetOwner()->Transform()->GetWorldPos().y;

    if (NewHeight >= m_JumpHeight)
    {
        MRPFSM->ChangeState(L"MOVEG_HOVERDASH");
    }
}