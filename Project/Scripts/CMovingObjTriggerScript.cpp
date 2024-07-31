#include "pch.h"
#include "CMovingObjTriggerScript.h"

#include "CMovingObjScript.h"

CMovingObjTriggerScript::CMovingObjTriggerScript()
    : CScript(MOVINGOBJTRIGGERSCRIPT)
{
}

CMovingObjTriggerScript::CMovingObjTriggerScript(const CMovingObjTriggerScript& Origin)
    : CScript(Origin)
{
}

CMovingObjTriggerScript::~CMovingObjTriggerScript()
{
}

void CMovingObjTriggerScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (L"Main Player" == _OtherCollider->GetOwner()->GetName())
        GetOwner()->GetParent()->GetScript<CMovingObjScript>()->SetState(MovingObjState::Move);
}

void CMovingObjTriggerScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

UINT CMovingObjTriggerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CMovingObjTriggerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}