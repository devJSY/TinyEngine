#include "pch.h"
#include "CAttackAreaScript.h"

#include "CUnitScript.h"

CAttackAreaScript::CAttackAreaScript()
    : CMonsterATKScript(ATTACKAREASCRIPT)
{
}

CAttackAreaScript::~CAttackAreaScript()
{
}

void CAttackAreaScript::begin()
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
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (nullptr == pObj)
        return;

    UnitHit hitInfo = {};
    LAYER_PLAYER == pObj->GetLayerIdx() && L"Body Collider" == pObj->GetName() ? pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo)
                                                                               : void();
}

void CAttackAreaScript::OnTriggerExit(CCollider* _OtherCollider)
{
}