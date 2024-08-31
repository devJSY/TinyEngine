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
    void SetRatioSpeed(const float _fValue) { m_fRatioSpeed = _fValue; }
    void SetDashTime(const float _fValue) { m_fDashTime = _fValue; }
    void SetTextureSpeed(const float _fValue) { m_fTextureValueSpeed = _fValue; }
    void SetBackGroundAlpha(const float _fValue);

public:
    virtual void begin() override;
    virtual void tick() override;

    void ChangeState(DashEffectState _eState);

private:
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
