#pragma once
#include "CAsset.h"

#include "CState.h"
#include "CBlackboard.h"

class CStateMachine;

class CFSM : public CAsset
{
private:
    map<wstring, CState*> m_mapState;
    CBlackboard m_Blackboard;

public:
    void AddState(const wstring& _StateName, CState* _State);
    CState* FindState(const wstring& _StateName);

public:
    void finaltick(CStateMachine* _StateMachine);

public:
    virtual int Save(const wstring& _strRelativePath) override;
    virtual int Load(const wstring& _strFilePath) override;

    CLONE_DISABLE(CFSM);

public:
    CFSM(bool _bEngineAsset = false);
    virtual ~CFSM();
};
