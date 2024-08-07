#include "pch.h"
#include "CElfilisBig_Roar.h"

CElfilisBig_Roar::CElfilisBig_Roar()
    : m_AccTime(0.f)
{
}

CElfilisBig_Roar::~CElfilisBig_Roar()
{
}

void CElfilisBig_Roar::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CElfilisBig_Roar::Enter()
{
    GetOwner()->SetActive(true);
    GetOwner()->Animator()->Play(ANIMPREFIX("BigRoar"), false);

    //@ Effect 나타나는 Burning 효과
    m_AccTime = 0.f;
}

void CElfilisBig_Roar::Exit()
{
}