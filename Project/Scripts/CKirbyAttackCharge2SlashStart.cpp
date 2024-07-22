#include "pch.h"
#include "CKirbyAttackCharge2SlashStart.h"

CKirbyAttackCharge2SlashStart::CKirbyAttackCharge2SlashStart()
    : m_PrevSpeed(0.f)
{
}

CKirbyAttackCharge2SlashStart::~CKirbyAttackCharge2SlashStart()
{
}

void CKirbyAttackCharge2SlashStart::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"ATTACK_CHARGE2_SLASH");
    }
}

void CKirbyAttackCharge2SlashStart::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SuperSpinSlashStart"), false, false, 1.5f);

    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(3.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyAttackCharge2SlashStart::Exit()
{
    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}