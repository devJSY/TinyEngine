#include "pch.h"
#include "CKirbyLanding.h"

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
    GetOwner()->Animator()->Play(KIRBYANIM(L"Landing"), false);
}

void CKirbyLanding::Exit()
{
}
