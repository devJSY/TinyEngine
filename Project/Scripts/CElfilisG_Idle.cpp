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
         ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundAtk, L"GROUND_ATK_SWORDWAVE_STORM");
    }
    if (KEY_TAP(KEY::ENTER))
    {
        ELFFSM->ChangeStateGroup_RandState(ElfilisStateGroup::GroundAtk);
    }

    //@TODO 테스트 후 복구
    return;
    m_AccTime += DT;

    if (m_AccTime >= 1.f)
    {
        ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundMove, L"Teleport");
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
