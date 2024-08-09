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
}

void CElfilisG_Idle::Enter_Step()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("Wait"));
    m_AccTime = 0.f;
}

void CElfilisG_Idle::Exit_Step()
{
}
