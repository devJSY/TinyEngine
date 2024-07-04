#include "pch.h"
#include "CKirbyHoveringSpit.h"

CKirbyHoveringSpit::CKirbyHoveringSpit()
{
}

CKirbyHoveringSpit::~CKirbyHoveringSpit()
{
}

void CKirbyHoveringSpit::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"JUMP_FALL");
    }
}

void CKirbyHoveringSpit::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"SpitAir"), false);
}

void CKirbyHoveringSpit::Exit()
{
    PLAYERFSM->SetHovering(false);
}