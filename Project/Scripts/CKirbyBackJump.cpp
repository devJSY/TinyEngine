#include "pch.h"
#include "CKirbyBackJump.h"
#include "CKirbyUnitScript.h"

CKirbyBackJump::CKirbyBackJump()
    : m_JumpPower(8.f)
    , m_InitSpeed(9.f)
    , m_PrevGravity(0.f)
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
        if (PLAYERCTRL->IsGround())
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
        if (PLAYER->Animator()->IsFinish())
        {
            if (PLAYERCTRL->IsGround())
            {
                ChangeState(L"IDLE");
            }
            else
            {
                ChangeState(L"JUMP_FALL");
            }
        }
    }
}

void CKirbyBackJump::Enter()
{
    if (PLAYERFSM->GetCurObjectIdx() == ObjectCopyType::NONE)
    {
        PLAYER->Animator()->Play(ANIMPREFIX("BackJump"), false, false, 2.f);

        if (PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::SWORD)
        {
            PLAYERFSM->LockSlideCombo();

            if (PLAYERFSM->GetSlideComboLevel() == 2)
            {
                PLAYERFSM->SetInvincible(true);
            }
        }
    }

    Vec3 KnockBackDir = PLAYERFSM->GetKnockBackDir();
    PLAYERFSM->SetKnockBackDir(Vec3());

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERCTRL->SetFriction(0.f);
    PLAYERCTRL->SetFrictionMode(true);

    PLAYERCTRL->AddVelocity({0.f, m_JumpPower, 0.f});
    PLAYERCTRL->AddVelocity(KnockBackDir * m_InitSpeed);

    m_PrevGravity = PLAYERCTRL->GetGravity();
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

    if (PLAYERFSM->GetCurObjectIdx() == ObjectCopyType::NONE)
    {
        PLAYER->Animator()->Play(ANIMPREFIX("BackJump"), false, false, 2.f);

        if (PLAYERFSM->GetCurAbilityIdx() == AbilityCopyType::SWORD)
        {
            PLAYERFSM->UnlockSlideCombo();

            if (PLAYERFSM->GetSlideComboLevel() == 2)
            {
                PLAYERFSM->SetSlideComboLevel(0);
                PLAYERFSM->SetInvincible(false);
            }
        }
    }

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERCTRL->SetFriction(1.f);
    PLAYERCTRL->SetFrictionMode(false);

    PLAYERCTRL->SetGravity(m_PrevGravity);
}
