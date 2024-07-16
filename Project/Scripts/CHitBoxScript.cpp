#include "pch.h"
#include "CHitBoxScript.h"

#include "CMonsterUnitScript.h"

void CHitBoxScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* _obj = _OtherCollider->GetOwner();
    // TODO : 해당 데미지 가지고 오기
    float _fDamage = 0.f;

    CMonsterUnitScript* _owner = GetOwner()->GetParent()->GetScript<CMonsterUnitScript>();

    _obj->GetLayerIdx() == LAYER_PLAYER ? _owner->GetDamage(_fDamage) : _owner->SetTarget(nullptr);
}

void CHitBoxScript::OnTriggerExit(CCollider* _OtherCollider)
{
    GetOwner()->GetParent()->GetScript<CMonsterUnitScript>()->SetTarget(nullptr);
}