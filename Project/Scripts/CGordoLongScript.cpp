#include "pch.h"
#include "CGordoLongScript.h"

CGordoLongScript::CGordoLongScript()
    : CMonsterUnitScript(GORDOLONGSCRIPT)
{
}

CGordoLongScript::~CGordoLongScript()
{
}

void CGordoLongScript::begin()
{
    Animator()->Play(ANIMPREFIX("Wait"));
}

void CGordoLongScript::tick()
{
    Rotating();
}

UINT CGordoLongScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CGordoLongScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);

    return MemoryByte;
}

void CGordoLongScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    (LAYER_PLAYER == pObj->GetLayerIdx() && L"Body Collider" == pObj->GetName()) ? pObj->GetScript<CUnitScript>()->GetDamage(GetHitInfo()) : void();
}

void CGordoLongScript::OnTriggerExit(CCollider* _OtherCollider)
{
}
