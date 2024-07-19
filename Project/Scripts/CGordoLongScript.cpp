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

void CGordoLongScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CGordoLongScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}

void CGordoLongScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    (LAYER_PLAYER == pObj->GetLayerIdx() && L"Body Collider" == pObj->GetName()) ? pObj->GetScript<CUnitScript>()->GetDamage(GetHitInfo()) : void();
}

void CGordoLongScript::OnTriggerExit(CCollider* _OtherCollider)
{
}
