#include "pch.h"
#include "CHitBoxScript.h"

#include "CMonsterUnitScript.h"

CHitBoxScript::CHitBoxScript()
    : CScript(HITBOXSCRIPT)
{
}

CHitBoxScript::~CHitBoxScript()
{
}

void CHitBoxScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    // TODO : 해당 데미지 가지고 오기
    UnitHit Hit;
    float fDamage = 0.f;

    Hit.Damage = fDamage;

    CMonsterUnitScript* OwnerScript = GetOwner()->GetParent()->GetScript<CMonsterUnitScript>();

    pObj->GetLayerIdx() == LAYER_PLAYER ? OwnerScript->GetDamage(Hit) : OwnerScript->SetTarget(nullptr);
}

void CHitBoxScript::OnTriggerExit(CCollider* _OtherCollider)
{
    GetOwner()->GetParent()->GetScript<CMonsterUnitScript>()->SetTarget(nullptr);
}

void CHitBoxScript::SaveToLevelFile(FILE* _File)
{
}

void CHitBoxScript::LoadFromLevelFile(FILE* _File)
{
}