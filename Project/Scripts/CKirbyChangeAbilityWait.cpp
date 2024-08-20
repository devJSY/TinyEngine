#include "pch.h"
#include "CKirbyChangeAbilityWait.h"

CKirbyChangeAbilityWait::CKirbyChangeAbilityWait()
{
}

CKirbyChangeAbilityWait::~CKirbyChangeAbilityWait()
{
}

void CKirbyChangeAbilityWait::tick()
{
    if (KEY_TAP(KEY_ATK))
    {
        PLAYERFSM->SetGlobalState(false);
        ChangeState(L"CHANGE_ABILITY_END");
    }
}

void CKirbyChangeAbilityWait::Enter()
{
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("EvolutionCopyWait"), true, false, 1.5f);

}

void CKirbyChangeAbilityWait::Exit()
{
    PLAYERFSM->SetInvincible(false);
}
