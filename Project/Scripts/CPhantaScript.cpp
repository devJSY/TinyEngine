#include "pch.h"
#include "CPhantaScript.h"

CPhantaScript::CPhantaScript()
    : CMonsterUnitScript(PHANTASCRIPT)
{
}

CPhantaScript::CPhantaScript(const CPhantaScript& Origin)
    : CMonsterUnitScript(Origin)
{
}

CPhantaScript::~CPhantaScript()
{
}

void CPhantaScript::begin()
{
}

void CPhantaScript::tick()
{
}

UINT CPhantaScript::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CPhantaScript::LoadFromLevelFile(FILE* _File)
{
    return 0;
}

void CPhantaScript::ChangeState(PhantaState _state)
{
}

void CPhantaScript::EnterState(PhantaState _state)
{
}

void CPhantaScript::ExitState(PhantaState _state)
{
}

void CPhantaScript::OnTriggerEnter(CCollider* _OtherCollider)
{
}

void CPhantaScript::OnTriggerExit(CCollider* _OtherCollider)
{
}