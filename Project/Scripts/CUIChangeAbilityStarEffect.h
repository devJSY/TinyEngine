#pragma once
#include <Engine\CScript.h>

enum class UIChangeAbilityStarState
{
    Move,
    Stop,
    End,
};

class CUIChangeAbilityStarEffect : public CScript
{
private:
    UIChangeAbilityStarState m_eState;
    Vec3 m_vCenterPos;

    float m_fOriginRadius;
    float m_fRadiusSpeeed;
    float m_fTheta;
    float m_fOriginTheta;
    float m_fThetaSpeed;

    float m_fPrevTheta;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(UIChangeAbilityStarState _eState) { m_eState = _eState; }
    void Move();
    void Stop();

    void SpawnStar();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIChangeAbilityStarEffect)
public:
    CUIChangeAbilityStarEffect();
    CUIChangeAbilityStarEffect(const CUIChangeAbilityStarEffect& Origin);
    virtual ~CUIChangeAbilityStarEffect();
};
