#pragma once
#include <Engine\\CScript.h>
class CState;
class CUnitScript;

class CFSMScript : public CScript
{
protected:
    CUnitScript* m_Unit;
    bool m_bGlobalState;

private:
    map<wstring, CState*> m_mapState;
    CState* m_CurState;
    CState* m_PrevState;

public:
    virtual void begin() override;
    virtual void tick() override;

    void AddState(const wstring& _StateName, CState* _State);
    CState* FindState(const wstring& _StateName);
    virtual void ChangeState(const wstring& _strStateName);
    void DeleteState(const wstring& _StateKey);

protected:
    void ChangeState_proc(CState* _pNextState);

public:
    void SetGlobalState(bool _GlobalState) { m_bGlobalState = _GlobalState; }
    void LockChangeState() { m_bGlobalState = true; }
    void UnlockChangeState() { m_bGlobalState = false; }

    virtual CUnitScript* GetUnit() { return m_Unit; }
    const map<wstring, CState*>& GetStates() { return m_mapState; }
    CState* GetCurState() { return m_CurState; }
    CState* GetPrevState() { return m_PrevState; }
    bool IsGlobalState() { return m_bGlobalState; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CFSMScript* Clone() = 0;

public:
    CFSMScript(SCRIPT_TYPE _ScriptType);
    CFSMScript(const CFSMScript& _Origin);
    virtual ~CFSMScript();
};