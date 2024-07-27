#include "pch.h"
#include "CElfilisG_Idle.h"
#include "CElfilisFSM.h"

CElfilisG_Idle::CElfilisG_Idle()
    : m_AccTime(0.f)
{
}

CElfilisG_Idle::~CElfilisG_Idle()
{
}

void CElfilisG_Idle::tick()
{
    m_AccTime += DT;

    if (m_AccTime >= 3.f)
    {
        ELFFSM->ChangeStateGroup_RandState(ElfilisStateGroup::GroundAtk);
        //ELFFSM->ChangeStateGroup_RandState(ElfilisStateGroup::GroundMove);
    }
}

void CElfilisG_Idle::Enter_Step()
{
    BOSS->Animator()->Play(ANIMPREFIX("Wait"));
    m_AccTime = 0.f;
}

void CElfilisG_Idle::Exit_Step()
{
}
