#include "pch.h"
#include "CKirbyAttackCharge2Run.h"

CKirbyAttackCharge2Run::CKirbyAttackCharge2Run()
{
}

CKirbyAttackCharge2Run::~CKirbyAttackCharge2Run()
{
}

void CKirbyAttackCharge2Run::tick()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCharge2Run();

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
            ChangeState(L"ATTACK_CHARGE2");
        }
    }
    break;
    case AbilityCopyType::FIRE: // 상태 없음
        break;
    case AbilityCopyType::RANGER:
        break;
    case AbilityCopyType::SWORD:
        break;
    }
}

void CKirbyAttackCharge2Run::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCharge2RunEnter();
}

void CKirbyAttackCharge2Run::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->AttackCharge2RunExit();
}