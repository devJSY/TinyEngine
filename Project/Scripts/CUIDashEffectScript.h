#pragma once
#include <Engine\CScript.h>

enum class DashEffectState
{
    Start,
    Progress,
    Stop,
    End,
};

class CUIDashEffectScript : public CScript
{
private:
    DashEffectState m_eState;

    float m_fRatio;

    float m_fDashTime;
    float m_fAccTime;

    float m_fRatioSpeed;
    float m_fTextureValue;
    float m_fTextureValueSpeed;
    float m_fMaxThreshHold;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(DashEffectState _eState);
    void Enter();
    void Exit();

    void Start();
    void Progress();
    void Stop();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIDashEffectScript)
public:
    CUIDashEffectScript();
    CUIDashEffectScript(const CUIDashEffectScript& Origin);
    virtual ~CUIDashEffectScript();
};
