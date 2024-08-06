#include "pch.h"
#include "CKirbyLongDiveBound.h"

CKirbyLongDiveBound::CKirbyLongDiveBound()
{
}

CKirbyLongDiveBound::~CKirbyLongDiveBound()
{
}
void CKirbyLongDiveBound::tick()
{
    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::NORMAL: {
        if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
        {
            ChangeState(L"ATTACK_CHARGE1_START");
        }
        else if (PLAYERCTRL->IsGround() && PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"IDLE");
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
        else if (PLAYERCTRL->IsGround() && PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"IDLE");
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
        else if (PLAYERCTRL->IsGround() && PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"IDLE");
        }
    }
    break;
    case AbilityCopyType::SWORD: {
        if (PLAYERFSM->GetSlideComboLevel())
        {
            if (PLAYERCTRL->IsGround())
            {
                ChangeState(L"IDLE");
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
        else if (PLAYERCTRL->IsGround() && PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"IDLE");
        }
    }
    break;
    case AbilityCopyType::SLEEP: {
        if (PLAYERCTRL->IsGround() && PLAYER->Animator()->IsFinish())
        {
            ChangeState(L"IDLE");
        }
    }
    break;
    }


    
}

void CKirbyLongDiveBound::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("LongDiveAttackBound"), false, false, 2.f);
}

void CKirbyLongDiveBound::Exit()
{
}

