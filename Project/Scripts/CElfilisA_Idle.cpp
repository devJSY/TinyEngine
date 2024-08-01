#include "pch.h"
#include "CElfilisA_Idle.h"
#include "CElfilisFSM.h"

CElfilisA_Idle::CElfilisA_Idle()
    : m_AccTime(0.f)
{
}

CElfilisA_Idle::~CElfilisA_Idle()
{
}

void CElfilisA_Idle::tick()
{
    if (KEY_TAP(KEY::ENTER))
    {
        ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::AirSmallAtk, L"AIR_ATKS_RAYARROW_UP");
        //ELFFSM->ChangeStateGroup_RandState(ElfilisStateGroup::AirMove);
    }

    //@TODO 전이 확인 후 복구
    return;
    m_AccTime += DT;

    if (m_AccTime >= 1.f)
    {
        ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundMove, L"GROUND_MOVE_TELEPORT");
    }
}

void CElfilisA_Idle::Enter_Step()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("WaitAir"));
    m_AccTime = 0.f;
}

void CElfilisA_Idle::Exit_Step()
{
}