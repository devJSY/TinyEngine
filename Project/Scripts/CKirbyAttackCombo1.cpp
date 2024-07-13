#include "pch.h"
#include "CKirbyAttackCombo1.h"

CKirbyAttackCombo1::CKirbyAttackCombo1()
{
}

CKirbyAttackCombo1::~CKirbyAttackCombo1()
{
}

void CKirbyAttackCombo1::tick()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCombo1();

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
                ChangeState(L"ATTACK_COMBO2");
            }
            //else if (KEY_PRESSED(KEY_ATK))
            //{
            //    ChangeState(L"ATTACK_CHARGE1_START");
            //}
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

void CKirbyAttackCombo1::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCombo1Enter();
}

void CKirbyAttackCombo1::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCombo1Exit();
}