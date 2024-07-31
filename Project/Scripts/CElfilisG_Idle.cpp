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
    if (KEY_TAP(KEY::SPACE))
    {
        //ELFFSM->ChangeStateGroup_RandState(ElfilisStateGroup::GroundMove);
         ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundToAir, L"GROUND_TOAIR");
    }
    if (KEY_TAP(KEY::ENTER))
    {
        ELFFSM->ChangeStateGroup_RandState(ElfilisStateGroup::GroundAtkFar);
    }

    m_AccTime += DT;

    if (m_AccTime >= 1.f)
    {
        ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundMove, L"GROUND_MOVE_TELEPORT");
    }
}

void CElfilisG_Idle::Enter_Step()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("Wait"));
    m_AccTime = 0.f;
}

void CElfilisG_Idle::Exit_Step()
{
}
