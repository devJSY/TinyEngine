#include "pch.h"
#include "CKirbySlideAttack.h"

CKirbySlideAttack::CKirbySlideAttack()
    : m_PlayTime(0.3f)
{
}

CKirbySlideAttack::~CKirbySlideAttack()
{
}

void CKirbySlideAttack::tick()
{
    m_PlayTime -= DT;

    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideAttack();

    // State Change
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL:
    case AbilityCopyType::FIRE:
    case AbilityCopyType::CUTTER:
        break;
    case AbilityCopyType::SWORD:
        if (m_PlayTime < 0.f && GetOwner()->Animator()->IsFinish())
        {
            if (PLAYERCTRL->IsGround())
            {
                ChangeState(L"LANDING");
            }
            else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                ChangeState(L"JUMP_ATTACK_START");
            }
            else if (KEY_TAP(KEY_JUMP))
            {
                ChangeState(L"HOVERING_START");
            }
            else
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;
    }
}

void CKirbySlideAttack::Enter()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideAttackEnter();

    PLAYERFSM->SetSlideComboLevel(1);
    PLAYERFSM->LockSlideCombo();
    PLAYERFSM->SetInvincible(true);

    m_PlayTime = 0.3f;
}

void CKirbySlideAttack::Exit()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    KirbyFSM->GetCurAbility()->SlideAttackExit();

    PLAYERFSM->UnlockSlideCombo();
    PLAYERFSM->SetInvincible(false);
}