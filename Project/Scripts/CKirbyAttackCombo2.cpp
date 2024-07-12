#include "pch.h"
#include "CKirbyAttackCombo2.h"

CKirbyAttackCombo2::CKirbyAttackCombo2()
{
}

CKirbyAttackCombo2::~CKirbyAttackCombo2()
{
}

void CKirbyAttackCombo2::tick()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCombo2();

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
            ChangeState(L"ATTACK_COMBO2_END");
        }
    }
    break;
    }
}

void CKirbyAttackCombo2::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCombo2Enter();
}

void CKirbyAttackCombo2::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCombo2Exit();
}