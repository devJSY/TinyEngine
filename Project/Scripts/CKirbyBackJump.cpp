#include "pch.h"
#include "CKirbyBackJump.h"

CKirbyBackJump::CKirbyBackJump()
    : m_JumpPower(8.f)
    , m_InitSpeed(9.f)
{
}

CKirbyBackJump::~CKirbyBackJump()
{
}

void CKirbyBackJump::tick()
{
    if (PLAYER->Animator()->IsFinish() && PLAYER->CharacterController()->IsGrounded())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyBackJump::Enter()
{
    Vec3 KnockBackDir = PLAYERFSM->GetKnockBackDir();

    PLAYER->Animator()->Play(ANIMPREFIX("BackJump"), false, false, 2.f);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERCTRL->SetFriction(0.f);
    PLAYERCTRL->SetGuard(true);

    PLAYERCTRL->AddVelocity({0.f, m_JumpPower, 0.f});
    PLAYERCTRL->AddVelocity(KnockBackDir * m_InitSpeed);

    PLAYERCTRL->SetGravity(-35.f);
}

void CKirbyBackJump::Exit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERCTRL->SetFriction(1.f);
    PLAYERCTRL->SetGuard(false);

    PLAYERCTRL->SetGravity(-20.f);
}
