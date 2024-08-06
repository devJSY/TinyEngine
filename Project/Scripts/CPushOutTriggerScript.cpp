#include "pch.h"
#include "CPushOutTriggerScript.h"

#include "CPushOutScript.h"

CPushOutTriggerScript::CPushOutTriggerScript()
    : CScript(PUSHOUTTRIGGERSCRIPT)
    , m_pPushOutObj(nullptr)
    , m_eState(CPushOutTriggerState::Wait)
    , m_iPushOutNum(-1)
    , m_fWaitTime(0.f)
    , m_fAccTime(0.f)
    , m_bEnabled(false)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_iPushOutNum, "Push Out Obj Num");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fWaitTime, "Wait Time");
}

CPushOutTriggerScript::CPushOutTriggerScript(const CPushOutTriggerScript& Origin)
    : CScript(Origin)
    , m_pPushOutObj(nullptr)
    , m_eState(CPushOutTriggerState::Wait)
    , m_iPushOutNum(-1)
    , m_bEnabled(Origin.m_bEnabled)
    , m_fWaitTime(Origin.m_fWaitTime)
    , m_fAccTime(Origin.m_fAccTime)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_iPushOutNum, "Push Out Obj Num");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fWaitTime, "Wait Time");
}

CPushOutTriggerScript::~CPushOutTriggerScript()
{
}

void CPushOutTriggerScript::begin()
{
    m_pPushOutObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PushOutObject" + std::to_wstring(m_iPushOutNum));
}

void CPushOutTriggerScript::tick()
{
    switch (m_eState)
    {
    case CPushOutTriggerState::Wait:
        break;
    case CPushOutTriggerState::Stop: {
        Stop();
    }
    break;
    case CPushOutTriggerState::End:
        break;
    default:
        break;
    }
}

void CPushOutTriggerScript::ChangeState(CPushOutTriggerState _state)
{
    ExitState();
    m_eState = _state;
    EnterState();
}

void CPushOutTriggerScript::EnterState()
{
}

void CPushOutTriggerScript::ExitState()
{
    switch (m_eState)
    {
    case CPushOutTriggerState::Wait: {
        SphereCollider()->SetEnabled(false);
    }
    break;
    }
}

void CPushOutTriggerScript::Stop()
{
    m_fAccTime += DT;
    if (m_fAccTime >= m_fWaitTime)
    {
        m_fAccTime = 0.f;
        SphereCollider()->SetEnabled(true);
        ChangeState(CPushOutTriggerState::Wait);
    }
}

void CPushOutTriggerScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        m_pPushOutObj->GetScript<CPushOutScript>()->ChangeState(PushOutState::MoveDest);
        ChangeState(CPushOutTriggerState::Stop);
    }
}

UINT CPushOutTriggerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_iPushOutNum, sizeof(int), 1, _File);
    fwrite(&m_fWaitTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(int);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CPushOutTriggerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_iPushOutNum, sizeof(int), 1, _File);
    fread(&m_fWaitTime, sizeof(float), 1, _File);

    MemoryByte += sizeof(int);
    MemoryByte += sizeof(float);

    return MemoryByte;
}