#include "pch.h"
#include "CBladeKnightScript.h"

CBladeKnightScript::CBladeKnightScript()
    : CMonsterUnitScript(BLADEKNIGHTSCRIPT)
{
}

CBladeKnightScript::CBladeKnightScript(const CBladeKnightScript& origin)
    : CMonsterUnitScript(origin)
{
}

CBladeKnightScript::~CBladeKnightScript()
{
}

void CBladeKnightScript::OnCollisionEnter(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnCollisionStay(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnCollisionExit(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnTriggerEnter(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnTriggerStay(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

void CBladeKnightScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CBladeKnightScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}
