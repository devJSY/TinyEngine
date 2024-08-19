#include "pch.h"
#include "CKirbyAttackCharge1Run.h"

CKirbyAttackCharge1Run::CKirbyAttackCharge1Run()
{
}

CKirbyAttackCharge1Run::~CKirbyAttackCharge1Run()
{
}

void CKirbyAttackCharge1Run::tick()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCharge1Run();

    // State Change
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL:
        break;
    case AbilityCopyType::FIRE: {
        if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
        {
            ChangeState(L"ATTACK_CHARGE1_END");
        }
        else if (PLAYERCTRL->GetInput().Length() == 0.f)
        {
            ChangeState(L"ATTACK_CHARGE1");
        }
    }
    break;
    case AbilityCopyType::CUTTER:
        break;
    case AbilityCopyType::SWORD:
        break;
    }
}

void CKirbyAttackCharge1Run::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCharge1RunEnter();
}

void CKirbyAttackCharge1Run::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCharge1RunExit();
}