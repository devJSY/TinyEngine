#include "pch.h"
#include "CKirbyAttackCharge1.h"

#include "CKirbyFSM.h"
#include "CPlayerMgr.h"
#include "CKirbyObject.h"
#include "CKirbyAbility.h"

CKirbyAttackCharge1::CKirbyAttackCharge1()
{
}

CKirbyAttackCharge1::~CKirbyAttackCharge1()
{
}

void CKirbyAttackCharge1::tick()
{
    wstring NextState = L"IDLE";

    PLAY_CURSTATE(AttackCharge1)

    // 기본적으로 수행해야 하는 동작

    // Change State
    // if (KEY_TAP(KEY::Q))
    //    NextState = L"ATTACK_COMBO";

    ChangeState(NextState);
}

void CKirbyAttackCharge1::Enter()
{
    PLAY_CURSTATE(AttackCharge1Enter)
}

void CKirbyAttackCharge1::Exit()
{
    PLAY_CURSTATE(AttackCharge1Exit)
}
