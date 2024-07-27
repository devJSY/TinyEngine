#include "pch.h"
#include "CElfilisG_Idle.h"
#include "CBossMgr.h"
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

    if (m_AccTime >= 1.f)
    {
        ELFFSM->ChangeStateGroup_RandState(ElfilisStateGroup::GroundMove);
    }
}

void CElfilisG_Idle::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Wait"));
    m_AccTime = 0.f;
}

void CElfilisG_Idle::Exit()
{
}