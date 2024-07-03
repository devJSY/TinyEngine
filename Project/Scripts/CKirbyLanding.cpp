#include "pch.h"
#include "CKirbyLanding.h"
#include "CKirbyMoveController.h"

CKirbyLanding::CKirbyLanding()
{
}

CKirbyLanding::~CKirbyLanding()
{
}

void CKirbyLanding::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"LANDING_END");
    }
}

void CKirbyLanding::Enter()
{
    if (PLAYERCTRL->GetJumpFallHeight() < 3.f)
    {
        GetOwner()->Animator()->Play(KIRBYANIM(L"LandingSmall"), false);
    }
    else
    {
        GetOwner()->Animator()->Play(KIRBYANIM(L"Landing"), false);
    }
}

void CKirbyLanding::Exit()
{
}
