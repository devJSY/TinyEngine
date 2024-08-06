#pragma once

#include <Engine/CScript.h>

enum class FlowState
{
    Start,
    Tick,
    End,
};

class CUIAnimScript;
class CUIFlowScript : public CScript
{
private:
    vector<CUIAnimScript*> m_vAnimScript;
    FlowState m_eState;

    float m_fWaitTime;
    float m_fAccTime;

    bool m_bIsFinish;
    bool m_bIsLoop;

public:
    void SetLoop(const bool _flag) { m_bIsLoop = _flag; }
    bool GetFinish() const { return m_bIsFinish; }

    void SetWaitTime(const float _time) { m_fWaitTime = _time; }

protected:
    void AddFlowScript(CUIAnimScript* _script);

public:
    virtual void tick() override;

private:
    void ChangeState(FlowState _state);
    void EnterState(FlowState _state);
    void ExitState(FlowState _state);

private:
    void EnterFlow();
    void TickFlow();
    void EndFlow();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIFlowScript)
public:
    CUIFlowScript();
    CUIFlowScript(UINT _iScriptType);
    virtual ~CUIFlowScript();
};
