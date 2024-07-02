#include "pch.h"
#include "CKirbyAttack.h"

CKirbyAttack::CKirbyAttack()
{
}

CKirbyAttack::~CKirbyAttack()
{
}

void CKirbyAttack::tick()
{
    //wstring NextState = L"IDLE";
    PLAY_CURSTATE(Attack)

    // 기본적으로 수행해야 하는 동작

    // Change State
    //if (KEY_TAP(KEY::Q))
    //    NextState = L"ATTACK_COMBO";

    //ChangeState(NextState);
}

void CKirbyAttack::Enter()
{
    PLAY_CURSTATE(AttackEnter)
}

void CKirbyAttack::Exit()
{
    PLAY_CURSTATE(AttackExit)
}