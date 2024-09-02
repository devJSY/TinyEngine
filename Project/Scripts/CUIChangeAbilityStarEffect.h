#pragma once
#include <Engine\CScript.h>

enum class UIChangeAbilityStarState
{
    GrowUp,
    GrowDown,
    Destroy,
    End,
};

class CUIChangeAbilityStarEffect : public CScript
{
private:
    UIChangeAbilityStarState m_eState;
    Vec3 m_vCenterPos;
    Vec3 m_vOriginScale;

    float m_fSpawnStarDeleteTime;

    float m_fOriginRadius;
    float m_fRadiusSpeed;
    float m_fTheta;
    float m_fOriginTheta;
    float m_fThetaSpeed;

    float m_fMaxThreshHold;

    float m_fStarSpawnTime;
    float m_fAccTime;
    
    float m_fBeginTheta;

    bool m_bLittleStar;

public:
    void InitializeSetting(const Vec3 _vCenterPos, const float _fOriginRadius, const float _fRadiusSpeed, const float _fTheta,
                           const float _fThetaSpeed, const float _fSpawnStarDeleteTime, const float _fMaxThresholdStar, const float _fStarSpawnTime,
                           const float _fBeginTheta);

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(UIChangeAbilityStarState _eState) { m_eState = _eState; }
    void GrowUp();
    void GrowDown();
    void Destroy();

    void SpawnStar();
    void ScalingAndRotating(const float _fRatio);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIChangeAbilityStarEffect)
public:
    CUIChangeAbilityStarEffect();
    CUIChangeAbilityStarEffect(const CUIChangeAbilityStarEffect& Origin);
    virtual ~CUIChangeAbilityStarEffect();
};
