#pragma once
#include <Engine\CScript.h>

enum class UIAnimState
{
    PrePared,
    Start,
    Tick,
    End,
};

class CUIAnimScript : public CScript
{
private:
    UIAnimState m_eState;
    float m_fWaitTime;
    float m_fAccTime;
    bool m_bIsFinsih;
    bool m_bLoopUI;

public:
    virtual void Enter(){};
    virtual void Idle(){};
    virtual void Exit(){};

public:
    UIAnimState GetUIAnimState() const { return m_eState; }
    void SetUIAnimState(const UIAnimState _state) { m_eState = _state; }

    void SetFinish(const bool _flag) { m_bIsFinsih = _flag; }
    bool GetFinish() { return m_bIsFinsih; }

    float GetWaitTime() const { return m_fWaitTime; }
    void SetAccTime(const float _fTime) { m_fAccTime = _fTime; }

    float GetAccTime() const { return m_fAccTime; }

    void SetLoopUI(const bool _flag) { m_bLoopUI = _flag; }
    bool GetLoopUI() const { return m_bLoopUI; }

public:
    virtual void begin() override;
    virtual void tick() override;


protected:
    void CommonUIEnter();
    void CommonUIExit();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIAnimScript)
public:
    CUIAnimScript();
    CUIAnimScript(UINT _ScriptType);
    CUIAnimScript(const CUIAnimScript& Origin);
    virtual ~CUIAnimScript();
};