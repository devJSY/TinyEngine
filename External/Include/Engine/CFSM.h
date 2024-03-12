#pragma once
#include "CAsset.h"

#include "CState.h"
#include "CBlackboard.h"

class CStateMachine;

class CFSM : public CAsset
{
private:
    CFSM* m_Master;
    CStateMachine* m_StateMachie; // FSM 을 사용하는 StateMachine;

    map<wstring, CState*> m_mapState;
    CBlackboard* m_Blackboard;

    CState* m_CurState;

public:
    void AddState(const wstring& _StateName, CState* _State);
    CState* FindState(const wstring& _StateName);
    void SetState(const wstring& _strState) { m_CurState = FindState(_strState); }
    void SetStateMachine(CStateMachine* _SM)
    {
        if (m_Master)
        {
            m_StateMachie = _SM;
        }
    }
    CFSM* GetFSMIstance();

    CStateMachine* GetStateMachine() const { return m_StateMachie; }
    void ChangeState(const wstring& _strStateName);

public:
    void finaltick();

public:
    virtual int Save(const wstring& _strRelativePath) override;
    virtual int Load(const wstring& _strFilePath) override;

    CLONE_DISABLE(CFSM);

public:
    CFSM(bool _bEngineAsset = false);
    virtual ~CFSM();
};
