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
}

void CKirbyAttackCharge1End::Enter()
{
    PLAY_CURSTATE(AttackCharge1EndEnter)
}

void CKirbyAttackCharge1End::Exit()
{
    PLAY_CURSTATE(AttackCharge1EndExit)
}

