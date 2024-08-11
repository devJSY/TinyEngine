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

    Vec3 vDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();
    UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, vDir.Normalize(), 6.f, 0.f, 0.f};
    L"Main Player" == pObj->GetName() ? pObj->GetScript<CUnitScript>()->GetDamage(hitInfo) : void();
}

void CAttackAreaScript::OnTriggerExit(CCollider* _OtherCollider)
{
}