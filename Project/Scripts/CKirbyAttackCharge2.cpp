#include "pch.h"
#include "CKirbyAttackCharge2.h"

CKirbyAttackCharge2::CKirbyAttackCharge2()
{
}

CKirbyAttackCharge2::~CKirbyAttackCharge2()
{
}

void CKirbyAttackCharge2::tick()
{
    PLAY_CURSTATE(AttackCharge2)
}

void CKirbyAttackCharge2::Enter()
{
    PLAY_CURSTATE(AttackCharge2Enter)
}

void CKirbyAttackCharge2::Exit()
{
    PLAY_CURSTATE(AttackCharge2Exit)
}
