#include "pch.h"
#include "CEnemyScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CEnemyScript::CEnemyScript()
    : CScript(ENEMYSCRIPT)
{
}

CEnemyScript::~CEnemyScript()
{
}

void CEnemyScript::begin()
{
}

void CEnemyScript::tick()
{
}

void CEnemyScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnCollisionExit(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnTriggerStay(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnTriggerExit(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::SaveToLevelFile(FILE* _File)
{
}

void CEnemyScript::LoadFromLevelFile(FILE* _File)
{
}
