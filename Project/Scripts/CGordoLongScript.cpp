#include "pch.h"
#include "CGordoLongScript.h"

#include "CPlayerMgr.h"
#include "CKirbyMoveController.h"

CGordoLongScript::CGordoLongScript()
    : CMonsterUnitScript(GORDOLONGSCRIPT)
{
    SetEatable(false);
}

CGordoLongScript::CGordoLongScript(const CGordoLongScript& Origin)
    : CMonsterUnitScript(Origin)
{
    SetEatable(false);
}

CGordoLongScript::~CGordoLongScript()
{
}

void CGordoLongScript::begin()
{
    CUnitScript::begin();
    Animator()->Play(ANIMPREFIX("Wait"), true, false, 1.5f);
}

void CGordoLongScript::tick()
{
    Rotating();
}

void CGordoLongScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (pObj->GetLayerIdx() == LAYER_PLAYER_TRIGGER && pObj->GetName() == L"Body Collider")
    {
        UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, Transform()->GetWorldDir(DIR_TYPE::FRONT), GetCurInfo().ATK, 0.f, 0.f};
        pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo);
    }
}

void CGordoLongScript::OnTriggerStay(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (pObj->GetLayerIdx() == LAYER_PLAYER_TRIGGER && pObj->GetName() == L"Body Collider")
    {
        PLAYERCTRL->PushAddMove(Vec3(-0.000001f, 0.f, 0.f));
    }
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