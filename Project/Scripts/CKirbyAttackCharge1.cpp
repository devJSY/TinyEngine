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
    PLAY_CURSTATE(AttackCharge1)
}

void CKirbyAttackCharge1::Enter()
{
    PLAY_CURSTATE(AttackCharge1Enter)
}

void CKirbyAttackCharge1::Exit()
{
    PLAY_CURSTATE(AttackCharge1Exit)
}
