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

    float m_fPrevTheta;

    float m_fMaxThreshHold;

    float m_fStarSpawnTime;
    float m_fAccTime;

    bool m_bLittleStar;

public:
    void SetCenterPos(const Vec3 _vCenterPos) { m_vCenterPos = _vCenterPos; }
    void SetOriginRadius(const float _fValue) { m_fOriginRadius = _fValue; }
    void SetRadiusSpeed(const float _fValue) { m_fRadiusSpeed = _fValue; }
    void SetTheta(const float _fValue) { m_fTheta = _fValue; }
    void SetThetaSpeed(const float _fValue) { m_fThetaSpeed = _fValue; }

    void SetSpawnStarDeleteTime(const float _fValue) { m_fSpawnStarDeleteTime = _fValue; }
    void SetMaxThreshHoldStar(const float _fValue) { m_fMaxThreshHold = _fValue; }

    void SetStarSpawnTime(const float _fValue) { m_fStarSpawnTime = _fValue; }

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
