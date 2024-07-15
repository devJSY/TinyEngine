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
#include "CNormalEnemySucessedAttack.h"
#include "CNormalEnemyFailedAttack.h"

void CNormalEnemyFSM::begin()
{
    AddState(L"IDLE", new CNormalEnemyIdle);
    AddState(L"PATROL", new CNormalEnemyPatrol);
    AddState(L"ATTACK", new CNormalEnemyAttack);
    AddState(L"FIND", new CNormalEnemyFind);
    AddState(L"SUCCESSEDATTACK", new CNormalEnemySucessedAttack);
    AddState(L"FAILEDATTACK", new CNormalEnemyFailedAttack);

    ChangeState(L"IDLE");
}

void CNormalEnemyFSM::tick()
{
    CFSMScript::tick();
}