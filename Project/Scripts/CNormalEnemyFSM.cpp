#include "pch.h"
#include "CNormalEnemyFSM.h"

CNormalEnemyFSM::CNormalEnemyFSM()
    : CFSMScript(NORMALENEMYFSM)
{
}

CNormalEnemyFSM::~CNormalEnemyFSM()
{
}

#include "CNormalEnemyIdle.h"
#include "CNormalEnemyPatrol.h"
#include "CNormalEnemyAttack.h"
#include "CNormalEnemyFind.h"

void CNormalEnemyFSM::begin()
{
    AddState(L"IDLE", new CNormalEnemyIdle);
    AddState(L"TRACK", new CNormalEnemyPatrol);
    AddState(L"ATTACK", new CNormalEnemyAttack);
    AddState(L"FIND", new CNormalEnemyFind);

    ChangeState(L"IDLE");
}

void CNormalEnemyFSM::tick()
{
    CFSMScript::tick();
}