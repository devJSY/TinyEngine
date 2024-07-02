#include "pch.h"
#include "CKirbyLandingEnd.h"

CKirbyLandingEnd::CKirbyLandingEnd()
{
}

CKirbyLandingEnd::~CKirbyLandingEnd()
{
}

void CKirbyLandingEnd::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyLandingEnd::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"LandingEnd"), false);
}

void CKirbyLandingEnd::Exit()
{
    // ¡∂¿€≈∞ ON
}
