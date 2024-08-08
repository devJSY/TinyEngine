#include "pch.h"
#include "CPushOutStopTriggerScript.h"

#include "CPushOutScript.h"
#include "CPushOutColliderScript.h"

CPushOutStopTriggerScript::CPushOutStopTriggerScript()
    : CScript(PUSHOUTSTOPTRIGGERSCRIPT)
    , m_bReturn(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bReturn, "Retrun Base");
}

CPushOutStopTriggerScript::CPushOutStopTriggerScript(const CPushOutStopTriggerScript& Origin)
    : CScript(Origin)
    , m_bReturn(false)
{
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bReturn, "Retrun Base");
}

CPushOutStopTriggerScript::~CPushOutStopTriggerScript()
{
}

void CPushOutStopTriggerScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (3 != pObj->GetLayerIdx())
        return;

    CPushOutScript* pushOutScript = pObj->GetScript<CPushOutScript>();
    
    if (nullptr != pushOutScript)
    {
        PushOutState state = m_bReturn == true ? PushOutState::MoveBase : PushOutState::Stop;
        pushOutScript->ChangeState(state);
    }
}

void CPushOutStopTriggerScript::OnTriggerStay(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();

    if (3 != pObj->GetLayerIdx())
        return;

    CPushOutScript* pushOutScript = pObj->GetScript<CPushOutScript>();

    if (nullptr != pushOutScript)
    {
        PushOutState state = m_bReturn == true ? PushOutState::MoveBase : PushOutState::Stop;
        pushOutScript->ChangeState(state);
    }
}

UINT CPushOutStopTriggerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_bReturn, sizeof(bool), 1, _File);

    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CPushOutStopTriggerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_bReturn, sizeof(bool), 1, _File);

    MemoryByte += sizeof(bool);

    return MemoryByte;
}