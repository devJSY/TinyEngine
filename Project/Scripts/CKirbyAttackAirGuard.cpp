#include "pch.h"
#include "CKirbyAttackAirGuard.h"

CKirbyAttackAirGuard::CKirbyAttackAirGuard()
    : m_PrevGravity(0.f)
    , m_PlayTime(1.f)
{
}

CKirbyAttackAirGuard::~CKirbyAttackAirGuard()
{
}

void CKirbyAttackAirGuard::tick()
{
    m_PlayTime -= DT;

    if (m_PlayTime < 0.f)
    {
        ChangeState(L"ATTACK_AIRGUARD_END");
    }
}

void CKirbyAttackAirGuard::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"ButterflyScalesAttack"), true, false, 1.5f, 0.2);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthOpen));
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);
    //@Effect Ãæ°ÝÆÄ & ¿Ö°î

    PLAYERCTRL->ClearVelocityY();
    m_PrevGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    m_PlayTime = 1.f;
}

void CKirbyAttackAirGuard::Exit()
{
    PLAYERCTRL->SetGravity(m_PrevGravity);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();
}