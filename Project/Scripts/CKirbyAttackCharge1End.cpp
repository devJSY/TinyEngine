#include "pch.h"
#include "CKirbyAttackCharge1End.h"

CKirbyAttackCharge1End::CKirbyAttackCharge1End()
{
}

CKirbyAttackCharge1End::~CKirbyAttackCharge1End()
{
}

void CKirbyAttackCharge1End::tick()
{
    PLAY_CURSTATE(AttackCharge1End)

    // State Change
    if (PLAYERFSM->GetCurObjectIdx() != ObjectCopyType::NONE)
    {
    }
    else
    {
        switch (PLAYERFSM->GetCurAbilityIdx())
        {
        case AbilityCopyType::NORMAL:
        case AbilityCopyType::SWORD:
        case AbilityCopyType::FIRE:
        {
            if (PLAYER->Animator()->IsFinish())
            {
                if (GetOwner()->CharacterController()->IsGrounded())
                {
                    ChangeState(L"IDLE");
                }
                else
                {
                    ChangeState(L"JUMP_FALL");
                }
            }
        }
        break;
        case AbilityCopyType::CUTTER:
            break;
        }
    }
}

void CKirbyAttackCharge1End::Enter()
{
    PLAY_CURSTATE(AttackCharge1EndEnter)
}

void CKirbyAttackCharge1End::Exit()
{
    PLAY_CURSTATE(AttackCharge1EndExit)
}

