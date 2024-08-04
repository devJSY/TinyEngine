#include "pch.h"
#include "CElfilisBig_Idle.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"

CElfilisBig_Idle::CElfilisBig_Idle()
{
}

CElfilisBig_Idle::~CElfilisBig_Idle()
{
}

void CElfilisBig_Idle::tick()
{
}

void CElfilisBig_Idle::Enter()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("BigWaitAir"), true, false, 4.f, 1.f);
}

void CElfilisBig_Idle::Exit()
{
}