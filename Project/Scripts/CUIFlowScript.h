#pragma once

#include <Engine/CScript.h>

enum class FlowState
{
    Start,
    Tick,
    End,
    Loading,
};

class CLevelFlowMgr;
class CUIAnimManagerScript;

class CUIFlowScript : public CScript
{
private:
    CLevelFlowMgr* m_pFlowMgr;
    CUIAnimManagerScript* m_pUIAnimManager;
    CGameObject* m_pLoadingObj;

    FlowState m_eState;

    float m_fWaitTime;
    float m_fAccTime;

    bool m_bIsFinish;
    bool m_bIsLoop;

public:
    void SetUIAnimManager(CUIAnimManagerScript* _pScript) { m_pUIAnimManager = _pScript; }

    void SetLoop(const bool _flag) { m_bIsLoop = _flag; }
    bool GetFinish() const { return m_bIsFinish; }

    void SetWaitTime(const float _time) { m_fWaitTime = _time; }
    void ChangeState(FlowState _state);

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void EnterState(FlowState _state);
    void ExitState(FlowState _state);

private:
    void EnterFlow();
    void TickFlow();
    void EndFlow();
    void LoadingFlow();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIFlowScript)
public:
    CUIFlowScript();
    CUIFlowScript(UINT _iScriptType);
    virtual ~CUIFlowScript();
};
