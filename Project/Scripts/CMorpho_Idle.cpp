#include "pch.h"
#include "CMorpho_Idle.h"

CMorpho_Idle::CMorpho_Idle()
{
}

CMorpho_Idle::~CMorpho_Idle()
{
}

void CMorpho_Idle::tick()
{
}

void CMorpho_Idle::Enter_Step()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("Wait"));
}

void CMorpho_Idle::Exit_Step()
{
}