#include "pch.h"
#include "CKirbyBackJump.h"
#include "CKirbyUnitScript.h"

CKirbyBackJump::CKirbyBackJump()
    : m_JumpPower(8.f)
    , m_InitSpeed(9.f)
    , m_StateEnter(true)
{
}

CKirbyBackJump::~CKirbyBackJump()
{
}

void CKirbyBackJump::tick()
{
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        //@TODO 회전시켜주기
        // Vec3 Rot = PLAYER->Transform()->GetLocalRotation();
        // Rot.x += DT * 10.f;
        // if (Rot.x >= XMConvertToRadians(360.f))
        //{
        //    Rot.x = XMConvertToRadians(360.f);
        //}

        if (PLAYER->CharacterController()->IsGrounded())
        {
            if (m_StateEnter)
            {
                PLAYERCTRL->ClearVelocityY();
                Vec3 Vel = PLAYERCTRL->GetVelocity() * -0.75f;
                PLAYERCTRL->AddVelocity(Vec3(Vel.x, 2.5f, Vel.z));
                m_StateEnter = false;
            }
            else
            {
                ChangeState(L"IDLE");
            }
        }
    }
    else
    {
        if (PLAYER->Animator()->IsFinish() && PLAYER->CharacterController()->IsGrounded())
        {
            ChangeState(L"IDLE");
        }
    }
}

void CKirbyBackJump::Enter()
{
    if (PLAYERFSM->GetCurObjectIdx() == ObjectCopyType::NONE)
    {
        PLAYER->Animator()->Play(ANIMPREFIX("BackJump"), false, false, 2.f);
    }

    Vec3 KnockBackDir = PLAYERFSM->GetKnockBackDir();
    PLAYERFSM->SetKnockBackDir(Vec3());

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERCTRL->SetFriction(0.f);
    PLAYERCTRL->SetGuard(true);

    PLAYERCTRL->AddVelocity({0.f, m_JumpPower, 0.f});
    PLAYERCTRL->AddVelocity(KnockBackDir * m_InitSpeed);

    PLAYERCTRL->SetGravity(-35.f);

    m_StateEnter = true;
}

void CKirbyBackJump::Exit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
    if (PLAYERFSM->GetCurObjectIdx() == ObjectCopyType::NONE)
    {
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    }

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERCTRL->SetFriction(1.f);
    PLAYERCTRL->SetGuard(false);

    PLAYERCTRL->SetGravity(-20.f);
}
