#include "pch.h"
#include "CKirbyLongDiveLanding.h"

CKirbyLongDiveLanding::CKirbyLongDiveLanding()
{
}

CKirbyLongDiveLanding::~CKirbyLongDiveLanding()
{
}


void CKirbyLongDiveLanding::tick()
{
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL: {
        if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
        {
            ChangeState(L"ATTACK_CHARGE1_START");
        }
        else if (PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    case AbilityCopyType::FIRE: {
        if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
        {
            if (KEY_PRESSED_ARROW && KEY_TAP(KEY_ATK))
            {
                ChangeState(L"BURNING_PRE");
            }
            else
            {
                ChangeState(L"ATTACK_CHARGE1_START");
            }
        }
        else if (PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    case AbilityCopyType::CUTTER: {
        if (KEY_TAP(KEY_ATK))
        {
            if (PLAYERFSM->CanBladeAttack())
            {
                ChangeState(L"ATTACK");
            }
        }
        else if (PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    case AbilityCopyType::SWORD: {
        if (PLAYERFSM->GetSlideComboLevel())
        {
            if (PLAYERCTRL->IsGround())
            {
            }
            else if ((KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK)) && PLAYERFSM->GetSlideComboLevel() == 1)
            {
                ChangeState(L"JUMP_ATTACK_START");
            }
        }
        else if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
        {
            if (KEY_TAP(KEY_GUARD) || KEY_PRESSED(KEY_GUARD))
            {
                ChangeState(L"ATTACK_AIRGUARD_CHARGE_START");
            }
            else
            {
                ChangeState(L"JUMP_ATTACK_START");
            }
        }
        else if (PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    case AbilityCopyType::SLEEP: {
        if (PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"LONGDIVE_BOUND");
        }
    }
    break;
    }

    
}

void CKirbyLongDiveLanding::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("LongDiveAttackLanding"), false, false, 2.f);

    PLAYERCTRL->AddVelocity(Vec3(0.f, 7.f, 0.f));

}

void CKirbyLongDiveLanding::Exit()
{
}
