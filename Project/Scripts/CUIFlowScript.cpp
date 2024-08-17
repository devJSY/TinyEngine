#include "pch.h"
#include "CUIFlowScript.h"

#include "CUIAnimScript.h"
#include "CLevelFlowMgr.h"

#include "CUIAnimManagerScript.h"

CUIFlowScript::CUIFlowScript()
    : CScript(UIFLOWSCRIPT)
    , m_pFlowMgr(nullptr)
    , m_pUIAnimManager(nullptr)
    , m_pLoadingObj(nullptr)
    , m_eState(FlowState::Start)
    , m_fWaitTime(0.f)
    , m_fAccTime(0.f)
    , m_bIsFinish(false)
    , m_bIsLoop(false)
{
}

CUIFlowScript::CUIFlowScript(UINT _iScriptType)
    : CScript(_iScriptType)
    , m_pFlowMgr(nullptr)
    , m_pUIAnimManager(nullptr)
    , m_pLoadingObj(nullptr)
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

void CUIFlowScript::begin()
{
    // TODO : Enter로 바꾸기
    m_pLoadingObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_Loading");

    if (nullptr != m_pLoadingObj)
    {
        m_pLoadingObj->SetActive(false);
    }

    m_pFlowMgr = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager")->GetScript<CLevelFlowMgr>();
    if (nullptr != m_pFlowMgr)
    {
        m_pFlowMgr->SetFlowScript(this);
    }

    ChangeState(FlowState::Start);
}

void CUIFlowScript::tick()
{
    switch (m_eState)
    {
    case FlowState::Start: {
        EnterFlow();
    }
    break;
    case FlowState::Tick: {
        TickFlow();
    }
    break;
    case FlowState::End: {
        EndFlow();
    }
    break;
    case FlowState::Loading: {
        LoadingFlow();
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
    switch (_state)
    {
    case FlowState::Start:
        break;
    case FlowState::Tick:
        break;
    case FlowState::End: {
        m_pUIAnimManager->EndAnimUI();
    }
    break;
    case FlowState::Loading:
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
}

void CUIFlowScript::TickFlow()
{
}

void CUIFlowScript::EndFlow()
{
    if (m_pUIAnimManager->AllFinishEndUI())
    {
        ChangeState(FlowState::Loading);
    }
}

void CUIFlowScript::LoadingFlow()
{
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