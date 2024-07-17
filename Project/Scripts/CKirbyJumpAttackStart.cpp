#include "pch.h"
#include "CKirbyJumpAttackStart.h"

CKirbyJumpAttackStart::CKirbyJumpAttackStart()
{
}

CKirbyJumpAttackStart::~CKirbyJumpAttackStart()
{
}

void CKirbyJumpAttackStart::tick()
{
    PLAY_CURSTATE(JumpAttackStart)

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
        case AbilityCopyType::CUTTER:
            break;
        case AbilityCopyType::SWORD: {
            if (PLAYER->Animator()->IsFinish())
            {
                if (GetOwner()->Animator()->IsFinish())
                {
                    ChangeState(L"JUMP_ATTACK");
                }
            }
        }
        break;
        }
    }
}

void CKirbyJumpAttackStart::Enter()
{
    PLAY_CURSTATE(JumpAttackStartEnter)
}

void CKirbyJumpAttackStart::Exit()
{
    PLAY_CURSTATE(JumpAttackStartExit)
}