#include "pch.h"
#include "CKirbyAttackAirGuardChargeStart.h"

CKirbyAttackAirGuardChargeStart::CKirbyAttackAirGuardChargeStart()
    : m_PrevGravity(0.f)
{
}

CKirbyAttackAirGuardChargeStart::~CKirbyAttackAirGuardChargeStart()
{
}

void CKirbyAttackAirGuardChargeStart::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"ATTACK_AIRGUARD_CHARGE");
    }
}

void CKirbyAttackAirGuardChargeStart::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX(L"ButterflyScalesChargeStart"), false, false, 10.f);
    //@Effect 차지 파티클입자

    PLAYERCTRL->ClearVelocityY();
    m_PrevGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();
}

void CKirbyAttackAirGuardChargeStart::Exit()
{
    PLAYERCTRL->SetGravity(m_PrevGravity);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();
}