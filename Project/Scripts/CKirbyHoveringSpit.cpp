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
        if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"LANDING");
        }
        ChangeState(L"JUMP_FALL");
    }
}

void CKirbyHoveringSpit::Enter()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("SpitAir"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    if (!PLAYERCTRL->IsGround())
    {
        PLAYERCTRL->AddVelocity(Vec3(0.f, 1.f, 0.f));
    }
    Vec3 InputWorld = PLAYERCTRL->GetInputWorld();
    if (InputWorld.Length())
        PLAYERCTRL->ForceDir({ForceDirType::DEFORM, InputWorld, false});

    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->SetSpeed(PLAYERUNIT->GetInitInfo().Speed / 3.f);

    PLAYERFSM->SetDroppable(true);
}

void CKirbyHoveringSpit::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERFSM->SetHovering(false);
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->SetSpeed(PLAYERUNIT->GetInitInfo().Speed);

    PLAYERFSM->SetDroppable(false);
}