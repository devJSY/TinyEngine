#include "pch.h"
#include "CKirbyAttackCombo2End.h"

CKirbyAttackCombo2End::CKirbyAttackCombo2End()
{
}

CKirbyAttackCombo2End::~CKirbyAttackCombo2End()
{
}

void CKirbyAttackCombo2End::tick()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCombo2End();

    // State Change
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL:
        break;
    case AbilityCopyType::FIRE:
        break;
    case AbilityCopyType::RANGER:
        break;
    case AbilityCopyType::SWORD: {
        if (PLAYER->Animator()->IsFinish())
        {
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                ChangeState(L"ATTACK");
            }
            else if (PLAYERCTRL->GetInput().Length() != 0.f)
            {
                ChangeState(L"RUN_START");
            }
            else
            {
                ChangeState(L"IDLE");
            }
        }
    }
    break;
    }
}

void CKirbyAttackCombo2End::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCombo2EndEnter();
}

void CKirbyAttackCombo2End::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCombo2EndExit();
}