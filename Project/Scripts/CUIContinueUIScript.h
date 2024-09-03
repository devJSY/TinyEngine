#pragma once
#include <Engine\CScript.h>

enum class UIContinueUIState
{
    Progress,
    End,
};

class CUIContinueUIScript : public CScript
{
private:
    UIContinueUIState m_eState;
    float m_fWaitTime;
    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Progress();
    void End();

public:
    void ChangeState(UIContinueUIState _eState) { m_eState = _eState; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIContinueUIScript)
public:
    CUIContinueUIScript();
    CUIContinueUIScript(const CUIContinueUIScript& Origin);
    virtual ~CUIContinueUIScript();
};
