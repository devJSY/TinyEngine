#include "pch.h"
#include "CState.h"
#include "CFSMScript.h"

CState::CState()
    : m_OwnerFSM(nullptr)
{
}

CState::CState(const CState& _Origin)
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
    if (!m_OwnerFSM)
    {
        assert(0);
        return;
    }

    m_OwnerFSM->ChangeState(_strStateName);
}