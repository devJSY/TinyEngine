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
#include "CNormalEnemyTrack.h"


void CNormalEnemyFSM::begin()
{
    AddState(L"IDLE", new CNormalEnemyIdle);
    AddState(L"TRACK", new CNormalEnemyTrack);

    ChangeState(L"IDLE");
}

void CNormalEnemyFSM::tick()
{
    CFSMScript::tick();
}