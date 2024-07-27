#include "pch.h"
#include "CBossState.h"

CBossState::CBossState()
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