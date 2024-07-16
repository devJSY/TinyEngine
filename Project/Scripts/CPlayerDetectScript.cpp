#include "pch.h"
#include "CPlayerDetectScript.h"

#include "CMonsterUnitScript.h"

void CPlayerDetectScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* _obj = _OtherCollider->GetOwner();
    CMonsterUnitScript* _owner = GetOwner()->GetParent()->GetScript<CMonsterUnitScript>();

    _obj->GetLayerIdx() == LAYER_PLAYER ? _owner->SetTarget(_obj) : _owner->SetTarget(nullptr);
}

void CPlayerDetectScript::OnTriggerExit(CCollider* _OtherCollider)
{
    GetOwner()->GetParent()->GetScript<CMonsterUnitScript>()->SetTarget(nullptr);
}