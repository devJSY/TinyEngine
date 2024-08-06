#include "pch.h"
#include "CUIFlowScript.h"

#include "CUIAnimScript.h"

CUIFlowScript::CUIFlowScript()
    : CScript(UIFLOWSCRIPT)
    , m_vAnimScript{}
    , m_eState(FlowState::Start)
    , m_fWaitTime(0.f)
    , m_fAccTime(0.f)
    , m_bIsFinish(false)
    , m_bIsLoop(false)
{
}

CUIFlowScript::CUIFlowScript(UINT _iScriptType)
    : CScript(_iScriptType)
    , m_vAnimScript{}
    , m_eState(FlowState::Start)
    , m_fWaitTime(0.f)
    , m_fAccTime(0.f)
    , m_bIsFinish(false)
    , m_bIsLoop(false)
{
}

CUIFlowScript::~CUIFlowScript()
{
}

void CUIFlowScript::tick()
{
    switch (m_eState)
    {
    case FlowState::Start: 
    {
        EnterFlow();
    }
        break;
    case FlowState::Tick:
    {
        TickFlow();
    }
    break;
    case FlowState::End:
    {
        EndFlow();
    }
        break;
    default:
        break;
    }
}

void CUIFlowScript::ChangeState(FlowState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CUIFlowScript::EnterState(FlowState _state)
{
    switch (m_eState)
    {
    case FlowState::End: {
        for (size_t i = 0; i < m_vAnimScript.size(); i++)
        {
            m_vAnimScript[i]->SetUIAnimState(UIAnimState::End);
        }
    }
    break;
    default:
        break;
    }
}

void CUIFlowScript::ExitState(FlowState _state)
{
}

void CUIFlowScript::EnterFlow()
{
    for (size_t i = 0; i < m_vAnimScript.size(); i++)
    {
        if (UIAnimState::Start == m_vAnimScript[i]->GetUIAnimState() || UIAnimState::PrePared == m_vAnimScript[i]->GetUIAnimState())
        {
            ChangeState(FlowState::Tick);
            break;
        }

        m_vAnimScript[i]->SetUIAnimState(UIAnimState::PrePared);
    }
}

void CUIFlowScript::TickFlow()
{
    bool flag = true;
    for (size_t i = 0; i < m_vAnimScript.size(); i++)
    {
        if (false == m_vAnimScript[i]->GetFinish())
        {
            flag = false;
        }
    }

    if (flag)
        ChangeState(FlowState::End);
}

void CUIFlowScript::EndFlow()
{
    if (m_bIsLoop)
    {
        m_fAccTime += DT;

        if (m_fAccTime >= m_fWaitTime)
        {
            m_eState = FlowState::Start;
            m_fAccTime = 0.f;
        }
    }
}

void CUIFlowScript::AddFlowScript(CUIAnimScript* _script)
{
    m_vAnimScript.push_back(_script);
}

UINT CUIFlowScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CUIFlowScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}