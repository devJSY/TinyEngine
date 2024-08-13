#include "pch.h"
#include "CKirbyRunEnd.h"

CKirbyRunEnd::CKirbyRunEnd()
{
}

CKirbyRunEnd::~CKirbyRunEnd()
{
}

void CKirbyRunEnd::tick()
{
    PLAY_CURSTATE(RunEnd);

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
        switch (PLAYERFSM->GetCurObjectIdx())
        {
        case ObjectCopyType::CONE:
        case ObjectCopyType::VENDING_MACHINE: {
            if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"IDLE");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
        }
        break;
        case ObjectCopyType::LIGHT: {
            if (PLAYERFSM->GetYPressedTime() >= PLAYERFSM->GetDropCopyTime())
            {
                ChangeState(L"DROP_OBJECT");
            }
            else if (KEY_TAP(KEY_ATK) && !PLAYERFSM->IsAttackEvent())
            {
                ChangeState(L"ATTACK");
            }
            else if ((KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK)) && PLAYERFSM->IsAttackEvent())
            {
                ChangeState(L"ATTACK_END");
            }
            else if ((KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP))) && (KEY_NONE(KEY_GUARD) || KEY_RELEASED(KEY_GUARD)))
            {
                ChangeState(L"JUMP_START");
            }
            else if (!PLAYERCTRL->IsGround())
            {
                ChangeState(L"JUMP_FALL");
            }
            else if (PLAYERCTRL->GetInput().Length() != 0.f)
            {
                ChangeState(L"RUN");
            }
            else if (GetOwner()->Animator()->IsFinish())
            {
                ChangeState(L"IDLE");
            }
        }
        break;
        }
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL:
        case AbilityCopyType::FIRE:
        case AbilityCopyType::CUTTER:
        case AbilityCopyType::SWORD:
            break;
        }
    }
}

void CKirbyRunEnd::Enter()
{
    PLAY_CURSTATE(RunEndEnter);
}

void CKirbyRunEnd::Exit()
{
    PLAY_CURSTATE(RunEndExit);
}