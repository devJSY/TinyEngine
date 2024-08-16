#include "pch.h"
#include "CAttackAreaScript.h"

#include "CPlayerMgr.h"

#include "CUnitScript.h"

CAttackAreaScript::CAttackAreaScript()
    : CMonsterATKScript(ATTACKAREASCRIPT)
{
}

CAttackAreaScript::~CAttackAreaScript()
{
}

UINT CAttackAreaScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterATKScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CAttackAreaScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterATKScript::LoadFromLevelFile(_File);

    return MemoryByte;
}

// Body Collider
void CAttackAreaScript::OnTriggerEnter(CCollider* _OtherCollider)
{
}

void CAttackAreaScript::OnTriggerExit(CCollider* _OtherCollider)
{
}