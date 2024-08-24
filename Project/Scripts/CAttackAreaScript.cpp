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

void CAttackAreaScript::OnTriggerStay(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (pObj->GetLayerIdx() == LAYER_PLAYER && pObj->GetName() == L"Main Player")
    {
        UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, Transform()->GetWorldDir(DIR_TYPE::FRONT), 8.f, 0.f, 0.f};
        pObj->GetScript<CUnitScript>()->GetDamage(hitInfo);
    }
}
