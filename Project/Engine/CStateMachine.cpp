#include "pch.h"
#include "CStateMachine.h"

CStateMachine::CStateMachine()
    : CComponent(COMPONENT_TYPE::STATEMACHINE)
    , m_FSM(nullptr)
    , m_Blackboard()
{
}

CStateMachine::~CStateMachine()
{
}

void CStateMachine::finaltick()
{
    if (nullptr == m_FSM)
        return;

    m_FSM->finaltick(this);
}

void CStateMachine::SetFSM(Ptr<CFSM> _FSM)
{
    m_FSM = _FSM;
}

void CStateMachine::AddBlackboardData(const wstring& _strKey, BLACKBOARD_DATA _Type, void* _pData)
{
    m_Blackboard.AddBlackboardData(_strKey, _Type, _pData);
}

void CStateMachine::SaveToLevelFile(FILE* _File)
{
}

void CStateMachine::LoadFromLevelFile(FILE* _File)
{
}
