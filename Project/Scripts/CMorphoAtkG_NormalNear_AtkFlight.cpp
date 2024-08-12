#include "pch.h"
#include "CMorphoAtkG_NormalNear_AtkFlight.h"
#include "CMorphoFSM.h"

CMorphoAtkG_NormalNear_AtkFlight::CMorphoAtkG_NormalNear_AtkFlight()
    : m_PrevDrag(0.f)
{
}

CMorphoAtkG_NormalNear_AtkFlight::~CMorphoAtkG_NormalNear_AtkFlight()
{
}

void CMorphoAtkG_NormalNear_AtkFlight::tick()
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

void CMorphoAtkG_NormalNear_AtkFlight::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("BackStepStart"), false, false, 0.5f, 1.f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AttackFlight"), false, false, 1.5f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("BackStepEnd"), false, false, 0.5f, 1.f);
    }
    break;
    }
}

void CMorphoAtkG_NormalNear_AtkFlight::Exit_Step()
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

void CMorphoAtkG_NormalNear_AtkFlight::Start()
{
    // Change Step
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkG_NormalNear_AtkFlight::Progress()
{
    // Change Step
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkG_NormalNear_AtkFlight::End()
{
    // Change Step
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}
