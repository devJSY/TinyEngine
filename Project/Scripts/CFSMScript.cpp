#include "pch.h"
#include "CFSMScript.h"
#include "CState.h"

#include <Engine/CTaskMgr.h>

CFSMScript::CFSMScript(SCRIPT_TYPE _ScriptType)
    : CScript(_ScriptType)
    , m_CurState(nullptr)
    , m_PrevState(nullptr)
    , m_bGlobalState(false)
{
}

CFSMScript::CFSMScript(const CFSMScript& _Origin)
    : CScript(_Origin)
    , m_CurState(nullptr)
    , m_PrevState(nullptr)
    , m_bGlobalState(false)
{
    map<wstring, CState*>::const_iterator iter = _Origin.m_mapState.begin();

    for (; iter != _Origin.m_mapState.end(); ++iter)
    {
        CState* pCloneState = iter->second->Clone();

        AddState(iter->second->GetName(), pCloneState);
    }

    if (nullptr != _Origin.m_CurState)
    {
        m_CurState = FindState(_Origin.m_CurState->GetName());
    }

}

CFSMScript::~CFSMScript()
{
    Delete_Map(m_mapState);
}



void CFSMScript::tick()
{
    if (m_CurState)
    {
        m_CurState->tick();
    }
}

void CFSMScript::AddState(const wstring& _StateName, CState* _State)
{
    if (FindState(_StateName))
        return;

    _State->m_OwnerFSM = this;

    _State->SetName(_StateName);
    m_mapState.insert(make_pair(_StateName, _State));
}

CState* CFSMScript::FindState(const wstring& _StateName)
{
    map<wstring, CState*>::iterator iter = m_mapState.find(_StateName);

    if (iter == m_mapState.end())
        return nullptr;

    return iter->second;
}

void CFSMScript::DeleteState(const wstring& _StateKey)
{
    CState* pState = FindState(_StateKey);

    if (!pState)
    {
        MessageBoxA(nullptr, "해당 키를 가진 스테이트가 없습니다.", "Delete State Failed", MB_OK);
        return;
    }

    delete pState;
    m_mapState.erase(_StateKey);
}

void CFSMScript::ChangeState(const wstring& _strState)
{
    if (m_bGlobalState)
        return;

    // 1. next state check
    CState* pNextState = FindState(_strState);
    assert(pNextState);

    ChangeState_proc(pNextState);

    // 2. adjust task to taskMgr
    // Param1: Parent Object    |   Param2: Next State
    //tTask pTask = {};
    //pTask.Type = TASK_TYPE::CHANGE_STATE;
    //pTask.Param_1 = (UINT_PTR)GetOwner();
    //pTask.Param_2 = (UINT_PTR)pNextState;
    //CTaskMgr::GetInst()->AddTask(pTask);
}

void CFSMScript::ChangeState_proc(CState* _pNextState)
{
    if (m_CurState)
    {
        m_CurState->Exit();
    }

    m_PrevState = m_CurState;
    m_CurState = _pNextState;
    m_CurState->Enter();

    LOG(LOG_LEVEL::Log, (string("[State Change] : ") + ToString(m_CurState->GetName())).c_str());
}

UINT CFSMScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CFSMScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
