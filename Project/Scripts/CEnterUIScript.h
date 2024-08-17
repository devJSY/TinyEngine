#pragma once
#include <Engine\CScript.h>

enum class EnterUIState
{
    None,
    Enter,
    FadeOut,
    End,
};

class CEnterUIScript : public CScript
{
private:
    vector<CGameObject*> m_pChildObj;
    EnterUIState m_eState;
    bool m_bIsFinish;
    
    float m_fFadeInTime;
    float m_fFadeOutTime;
    float m_fAccTime;

public:
    bool IsFinish() { return m_bIsFinish; }
    void ChangeState(EnterUIState _eState);

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void EnterState();
    void ExitState();

private:
    void Enter();
    void FadeOut();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CEnterUIScript)
public:
    CEnterUIScript();
    CEnterUIScript(const CEnterUIScript& Origin);
    virtual ~CEnterUIScript();
};
