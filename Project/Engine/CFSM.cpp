#include "pch.h"
#include "CFSM.h"

CFSM::CFSM(bool _bEngineAsset)
    : CAsset(ASSET_TYPE::FSM, _bEngineAsset)
    , m_mapState()
    , m_Blackboard()
{
}

CFSM::~CFSM()
{
}

void CFSM::finaltick(CStateMachine* _StateMachine)
{
}

void CFSM::AddState(const wstring& _StateName, CState* _State)
{
    assert(!(FindState(_StateName)));

    _State->m_FSM = this;
    m_mapState.insert(make_pair(_StateName, _State));
}

CState* CFSM::FindState(const wstring& _StateName)
{
    map<wstring, CState*>::iterator iter = m_mapState.find(_StateName);

    if (iter == m_mapState.end())
        return nullptr;

    return iter->second;
}
