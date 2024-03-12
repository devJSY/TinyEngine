#pragma once
#include "CEntity.h"

class CFSM;
class CStateMachine;

class CState : public CEntity
{
private:
    CFSM* m_FSM;

public:
    virtual void finaltick() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

protected:
    void* GetBlackboardData(const wstring& _strKey);
    CFSM* GetFSM() const { return m_FSM; }

    void ChangeState(const wstring& _strStateName);

public:
    CLONE_DISABLE(CState);

public:
    CState();
    virtual ~CState();

    friend class CFSM;
};
