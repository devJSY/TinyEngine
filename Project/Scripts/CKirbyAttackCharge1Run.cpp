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
    {
        if (PLAYERFSM->IsStuffed())
        {
            ChangeState(L"STUFFED");
        }
        else if (KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
        {
            ChangeState(L"ATTACK_CHARGE1_END");
        }
        else if (PLAYERCTRL->GetInput().Length() == 0.f)
        {
            ChangeState(L"ATTACK_CHARGE1");
        }
        else if (PLAYERFSM->GetChargeAccTime() >= PLAYERFSM->GetCurAbility()->GetCharge1Time())
        {
            ChangeState(L"ATTACK_CHARGE2_RUN");
        }
    }
    break;
    case AbilityCopyType::FIRE:
        break;
    case AbilityCopyType::RANGER:
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