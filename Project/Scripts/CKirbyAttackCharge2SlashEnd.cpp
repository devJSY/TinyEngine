#include "pch.h"
#include "CKirbyAttackCharge2SlashEnd.h"

CKirbyAttackCharge2SlashEnd::CKirbyAttackCharge2SlashEnd()
{
}

CKirbyAttackCharge2SlashEnd::~CKirbyAttackCharge2SlashEnd()
{
}

void CKirbyAttackCharge2SlashEnd::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyAttackCharge2SlashEnd::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SuperSpinSlashEnd"), false, false, 1.5f);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetFriction(4.f);
    PLAYERCTRL->SetFrictionMode(true);

    PLAYERFSM->SetInvincible(true);

    PLAYERFSM->GetCurWeapon()->GetChildObject(L"KirbySwordFireParticle")->ParticleSystem()->EnableModule(PARTICLE_MODULE::SPAWN, false);

    // sound
    wstring Sound = L"sound\\wav\\HeroSwordMorpho\\0003_Fire.wav";
    GamePlayStatic::Play2DSound(Sound, 1, SOUND_KIRBY * 0.8f);
}

void CKirbyAttackCharge2SlashEnd::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetFriction(0.f);
    PLAYERCTRL->SetFrictionMode(false);

    PLAYERFSM->SetInvincible(false);
}