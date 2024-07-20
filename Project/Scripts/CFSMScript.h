#pragma once
#include <Engine\\CScript.h>

class CState;

class CFSMScript : public CScript
{
private:
    map<wstring, CState*>           m_mapState; 
    CState*                         m_CurState; 
    CState*                         m_PrevState;

    bool                            m_bGlobalState;

public:
    virtual void begin() override = 0;
    virtual void tick() override;

    void AddState(const wstring& _StateName, CState* _State);
    CState* FindState(const wstring& _StateName);
    void ChangeState(const wstring& _strStateName);
    void DeleteState(const wstring& _StateKey);
    void SetGlobalState(bool _GlobalState) { m_bGlobalState = _GlobalState; }

private:
    void ChangeState_proc(CState* _pNextState);

public:
    const map<wstring, CState*>& GetStates() { return m_mapState; }
    CState* GetCurState() { return m_CurState; }
    CState* GetPrevState() { return m_PrevState; }
    bool IsGlobalState() { return m_bGlobalState; }


public:
    virtual UINT SaveToLevelFile(FILE* _File) {}
    virtual UINT LoadFromLevelFile(FILE* _File) {}

public:
    CFSMScript* Clone() = 0;

public:
    CFSMScript(SCRIPT_TYPE _ScriptType);
    CFSMScript(const CFSMScript& _Origin);
    virtual ~CFSMScript();
};