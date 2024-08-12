#include "pch.h"
#include "CBossState.h"

CBossState::CBossState()
    : m_Step(StateStep::Start)
{
}

CBossState::~CBossState()
{
}

void CBossState::Enter()
{
    m_Step = StateStep::Start;
    Enter_Step();
}

void CBossState::Exit()
{
    Exit_Step();
}

void CBossState::ChangeStep(StateStep _Step)
{
    Exit_Step();
    m_Step = _Step;
    Enter_Step();
}

void CBossState::RotateToPlayer(float delta)
{
    Vec3 Dir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
    Dir.y = 0.f;
    Dir.Normalize();

    if (delta == 0.f)
    {
        GetOwner()->Transform()->Slerp(Dir, DT * BOSSUNIT->GetCurInfo().RotationSpeed);
    }
    else
    {
        GetOwner()->Transform()->Slerp(Dir, delta);
    }
}
