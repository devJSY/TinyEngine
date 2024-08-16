#include "pch.h"
#include "CGordoLongScript.h"

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
    SetEatable(false);
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

    UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, Transform()->GetWorldDir(DIR_TYPE::FRONT), m_CurInfo.ATK, 0.f, 0.f};
    (LAYER_PLAYER == pObj->GetLayerIdx() && L"Main Player" == pObj->GetName()) ? pObj->GetScript<CUnitScript>()->GetDamage(hitInfo) : void();
}