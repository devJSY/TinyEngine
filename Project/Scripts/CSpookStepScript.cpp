#include "pch.h"
#include "CSpookStepScript.h"

CSpookStepScript::CSpookStepScript()
    : CMonsterUnitScript(SPOOKSTEPSCRIPT)
{
}

CSpookStepScript::CSpookStepScript(const CSpookStepScript& Origin)
    : CMonsterUnitScript(Origin)
{
}

CSpookStepScript::~CSpookStepScript()
{
}

void CSpookStepScript::begin()
{
}

void CSpookStepScript::tick()
{
}

UINT CSpookStepScript::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CSpookStepScript::LoadFromLevelFile(FILE* _File)
{
    return 0;
}

void CSpookStepScript::ChangeState(SpookStepState _state)
{
}

void CSpookStepScript::EnterState(SpookStepState _state)
{
}

void CSpookStepScript::ExitState(SpookStepState _state)
{
}

void CSpookStepScript::OnTriggerEnter(CCollider* _OtherCollider)
{
}

void CSpookStepScript::OnTriggerExit(CCollider* _OtherCollider)
{
}