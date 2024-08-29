#pragma once
#include <Engine\CScript.h>

enum class CUIChangeAbilityStopStarEffectState
{
    Progress,
    End,
};

class CUIChangeAbilityStopStarEffect : public CScript
{
private:
    CUIChangeAbilityStopStarEffectState m_eState;

    Vec3 m_vOriginScale;
    float m_fTheta;

    float m_fAccTime;
    float m_fDeleteTime;

public:
    void SetDeleteTime(float _fValue) { m_fDeleteTime = _fValue; }
    void SetTheta(float _fValue) { m_fTheta = _fValue; }

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(CUIChangeAbilityStopStarEffectState _eState) { m_eState = _eState; }
    void Progress();
    void End();

    void ScalingAndRotating(const float _fRatio);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIChangeAbilityStopStarEffect)
public:
    CUIChangeAbilityStopStarEffect();
    CUIChangeAbilityStopStarEffect(const CUIChangeAbilityStopStarEffect& Origin);
    virtual ~CUIChangeAbilityStopStarEffect();
};
