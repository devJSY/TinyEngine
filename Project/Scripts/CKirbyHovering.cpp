#include "pch.h"
#include "CKirbyHovering.h"

CKirbyHovering::CKirbyHovering()
{
}

CKirbyHovering::~CKirbyHovering()
{
}

void CKirbyHovering::tick()
{
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        ChangeState(L"HOVERING_SPIT");
    }
    else if (GetOwner()->CharacterController()->IsGrounded())
    {
        ChangeState(L"HOVERING_LANDING");
    }
    else if (PLAYERCTRL->GetInput().Length() == 0.f && (KEY_RELEASED(KEY_JUMP) || (KEY_NONE(KEY_JUMP))))
    {
        ChangeState(L"HOVERING_FALL");
    }
    else if (PLAYERFSM->GetHoveringAccTime() >= PLAYERFSM->GetHoveringLimitTime())
    {
        ChangeState(L"HOVERING_LIMIT");
    }
}

void CKirbyHovering::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"Flight"));
}

void CKirbyHovering::Exit()
{
}