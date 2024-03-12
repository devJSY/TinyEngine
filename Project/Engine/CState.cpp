#include "pch.h"
#include "CState.h"

#include "CStateMachine.h"

CState::CState()
    : m_FSM(nullptr)
{
}

CState::~CState()
{
}

void* CState::GetBlackboardData(const wstring& _strKey)
{
    CStateMachine* pSM = m_FSM->GetStateMachine();
    return pSM->GetBlackboardData(_strKey);
}

void CState::ChangeState(const wstring& _strStateName)
{
    assert(m_FSM);
    m_FSM->ChangeState(_strStateName);
}
