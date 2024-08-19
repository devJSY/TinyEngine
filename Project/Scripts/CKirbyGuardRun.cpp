#include "pch.h"
#include "CKirbyGuardRun.h"
#include "CMomentaryObjScript.h"

CKirbyGuardRun::CKirbyGuardRun()
    : m_PlayTime(1.f)
{
    m_SwordDodgeAttackPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordDodgeAttack.pref");
}

CKirbyGuardRun::~CKirbyGuardRun()
{
}

void CKirbyGuardRun::tick()
{
    m_PlayTime -= DT;

    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->GuardRun();

    // State Change
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL:
    case AbilityCopyType::FIRE:
    case AbilityCopyType::CUTTER:
        break;
    case AbilityCopyType::SWORD:{
        if (m_PlayTime < 0.f || PLAYERCTRL->GetInput().Length() == 0.f || KEY_RELEASED(KEY_GUARD) || KEY_NONE(KEY_GUARD))
        {
            ChangeState(L"GUARD");
        }
    }
        break;
    }
}

void CKirbyGuardRun::Enter()
{
    PLAYERFSM->GetCurAbility()->GuardRunEnter();
    PLAYERFSM->OffCollider();
    PLAYERFSM->SetInvincible(true);

    m_PlayTime = 1.f;
}

void CKirbyGuardRun::Exit()
{
    PLAYERFSM->GetCurAbility()->GuardRunExit();
    PLAYERFSM->OnCollider();
    PLAYERFSM->SetInvincible(false);

    if (m_SwordDodgeAttackPref != nullptr)
    {
        CGameObject* Attack = m_SwordDodgeAttackPref->Instantiate();
        Attack->Transform()->SetWorldPos(PLAYER->Transform()->GetWorldPos());

        CMomentaryObjScript* Script = Attack->GetScript<CMomentaryObjScript>();
        Script->SetPlayTime(0.2f);

        GamePlayStatic::SpawnGameObject(Attack, LAYER_PLAYERATK_TRIGGER);
    }
}