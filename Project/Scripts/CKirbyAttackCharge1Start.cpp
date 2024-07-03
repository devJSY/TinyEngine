#include "pch.h"
#include "CKirbyAttackCharge1Start.h"

CKirbyAttackCharge1Start::CKirbyAttackCharge1Start()
{
}

CKirbyAttackCharge1Start::~CKirbyAttackCharge1Start()
{
}

void CKirbyAttackCharge1Start::tick()
{
    PLAY_CURSTATE(AttackCharge1Start)
}

void CKirbyAttackCharge1Start::Enter()
{
    PLAY_CURSTATE(AttackCharge1StartEnter)
}

void CKirbyAttackCharge1Start::Exit()
{
    PLAY_CURSTATE(AttackCharge1StartExit)
}
