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
    Animator()->Play(ANIMPREFIX(L"Wait"));
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

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        pObj->GetScript<CUnitScript>()->GetDamage(GetHitInfo());
    }
}

void CGordoLongScript::OnTriggerExit(CCollider* _OtherCollider)
{
}
