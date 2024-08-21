#include "pch.h"
#include "CDetectAreaScript.h"
#include "CMonsterUnitScript.h"

CDetectAreaScript::CDetectAreaScript()
    : CScript(DETECTAREASCRIPT)
{
}

CDetectAreaScript::~CDetectAreaScript()
{
}

void CDetectAreaScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    // CDetectAreaScript를 보유하고있는 오브젝트가 자식오브젝트가 아닌경우
    if (!GetOwner()->GetParent())
        return;

    CGameObject* pObj = _OtherCollider->GetOwner(); 
    CMonsterUnitScript* OwnerScript = GetOwner()->GetParent()->GetScript<CMonsterUnitScript>();

    // CDetectAreaScript를 보유하고있는 오브젝트의 부모가 Monster가 아닌 경우
    if (nullptr == OwnerScript)
        return;

    // 플레이어 감지
    if (pObj->GetLayerIdx() == LAYER_PLAYER)
    {
        OwnerScript->SetTarget(pObj);
    }
}

void CDetectAreaScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (pObj->GetLayerIdx() == LAYER_PLAYER)
    {
        GetOwner()->GetParent()->GetScript<CMonsterUnitScript>()->SetTarget(nullptr);
    }
}

UINT CDetectAreaScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CDetectAreaScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
