#pragma once
#include "CComponent.h"

#include "CFSM.h"
#include "CBlackboard.h"

class CStateMachine : public CComponent
{
private:
    Ptr<CFSM> m_FSM;
    CBlackboard m_Blackboard;

public:
    void SetFSM(Ptr<CFSM> _FSM);
    void AddBlackboardData(const wstring& _strKey, BLACKBOARD_DATA _Type, void* _pData);

public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CStateMachine);

public:
    CStateMachine();
    virtual ~CStateMachine();
};
