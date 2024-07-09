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
        if (GetOwner()->CharacterController()->IsGrounded())
        {
            ChangeState(L"LANDING");
        }
        ChangeState(L"JUMP_FALL");
    }
}

void CKirbyHoveringSpit::Enter()
{
    GetOwner()->Animator()->Play(KIRBYANIM(L"SpitAir"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    if (!GetOwner()->CharacterController()->IsGrounded())
    {
        PLAYERCTRL->AddVelocity(Vec3(0.f, 1.f, 0.f));
    }
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockMove();
}

void CKirbyHoveringSpit::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERFSM->SetHovering(false);
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();
}