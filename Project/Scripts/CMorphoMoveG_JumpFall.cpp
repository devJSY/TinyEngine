#include "pch.h"
#include "CMorphoMoveG_JumpFall.h"
#include "CMorphoFSM.h"

CMorphoMoveG_JumpFall::CMorphoMoveG_JumpFall()
{
}

CMorphoMoveG_JumpFall::~CMorphoMoveG_JumpFall()
{
}

void CMorphoMoveG_JumpFall::tick()
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

void CMorphoMoveG_JumpFall::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Rigidbody()->SetUseGravity(true);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Landing"), false, false, 1.5f);

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\0001_Landing.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 1.3f);
    }
    break;
    }
}

void CMorphoMoveG_JumpFall::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::End:
        break;
    }
}

void CMorphoMoveG_JumpFall::Start()
{
    if (MRPFSM->IsGround())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoMoveG_JumpFall::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->Attack();
    }
}