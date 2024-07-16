#include "pch.h"
#include "CNormalEnemyFSM.h"

CNormalEnemyFSM::CNormalEnemyFSM()
    : CFSMScript(NORMALENEMYFSM)
{
}

CNormalEnemyFSM::~CNormalEnemyFSM()
{
}

#include "CEnemyIdle.h"
#include "CEnemyPatrol.h"
#include "CEnemyAttack.h"
#include "CEnemyFind.h"
#include "CEnemySuccessedAttack.h"
#include "CEnemyFailedAttack.h"

void CNormalEnemyFSM::begin()
{
    AddState(L"IDLE", new CEnemyIdle);
    AddState(L"PATROL", new CEnemyPatrol);
    AddState(L"ATTACK", new CEnemyAttack);
    AddState(L"FIND", new CEnemyFind);
    AddState(L"SUCCESSEDATTACK", new CEnemySuccessedAttack);
    AddState(L"FAILEDATTACK", new CEnemyFailedAttack);

    ChangeState(L"IDLE");
}

void CNormalEnemyFSM::tick()
{
    CFSMScript::tick();
}