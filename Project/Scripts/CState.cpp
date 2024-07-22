#include "pch.h"
#include "CState.h"
#include "CFSMScript.h"

CState::CState()
    : m_OwnerFSM(nullptr)
{
}

CState::~CState()
{
}

CGameObject* CState::GetOwner()
{
    return m_OwnerFSM->GetOwner();
}

void CState::ChangeState(const wstring& _strStateName)
{
    LOG(Log, "%s", ToString(_strStateName).c_str());

    assert(m_OwnerFSM);
    m_OwnerFSM->ChangeState(_strStateName);
}