#include "pch.h"
#include "CAttackAreaScript.h"

#include "CUnitScript.h"

CAttackAreaScript::CAttackAreaScript()
    : CMonsterATKScript(ATTACKAREATSCRIPT)
{
}

CAttackAreaScript::~CAttackAreaScript()
{
}

void CAttackAreaScript::begin()
{
}

void CAttackAreaScript::SaveToLevelFile(FILE* _File)
{
    CMonsterATKScript::SaveToLevelFile(_File);
}

void CAttackAreaScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterATKScript::LoadFromLevelFile(_File);
}

// Body Collider
void CAttackAreaScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (nullptr == pObj)
        return;

    LAYER_PLAYER == pObj->GetLayerIdx() && L"Body Collider" == pObj->GetName() 
        ? pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(m_HitInfo): void();
}

void CAttackAreaScript::OnTriggerExit(CCollider* _OtherCollider)
{
}