#include "pch.h"
#include "CKirbyRun.h"

#define RUN_SPEED 10.f

CKirbyRun::CKirbyRun()
{
}

CKirbyRun::~CKirbyRun()
{
}

void CKirbyRun::tick()
{
    PLAY_CURSTATE(Run)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL: 
        {
            if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
            {
                ChangeState(L"ATTACK_CHARGE1_START");
            }
            else if (KEY_TAP(KEY_JUMP) || (KEY_PRESSED(KEY_JUMP)))
            {
                ChangeState(L"JUMP_START");
            }
            else if (PLAYERCTRL->GetInput().Length() == 0.f)
            {
                ChangeState(L"IDLE");
            }
            else if (KEY_TAP(KEY_GUARD))
            {
                ChangeState(L"GUARD");
            }
        }
        break;

        case AbilityCopyType::FIRE:
        case AbilityCopyType::RANGER:
        case AbilityCopyType::SWORD:
            break;
        }
    }
}

void CKirbyRun::Enter()
{
    PLAY_CURSTATE(RunEnter)
}

void CKirbyRun::Exit()
{
    PLAY_CURSTATE(RunExit)
}