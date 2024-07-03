#include "pch.h"
#include "CKirbyIdle.h"

CKirbyIdle::CKirbyIdle()
{
}

CKirbyIdle::~CKirbyIdle()
{
}

void CKirbyIdle::tick()
{
    // 각 State별로 수행할 작업 (상태전환 제외)
    PLAY_CURSTATE(Idle)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL:
            break;
        case AbilityCopyType::FIRE:
            break;
        case AbilityCopyType::RANGER:
            break;
        case AbilityCopyType::SWORD:
            break;
        }
    }
}

void CKirbyIdle::Enter()
{
    PLAY_CURSTATE(IdleEnter)
}

void CKirbyIdle::Exit()
{
    PLAY_CURSTATE(IdleExit)
}