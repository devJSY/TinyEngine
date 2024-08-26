#pragma once
#include <Engine\CScript.h>

enum class DamageStarTickType
{
    Velocity,
    Direction,
    End,
};

enum class DamageStarSpawnType
{
    RandosPos,
    StaticPos,
    End,
};

enum class DamageStarState
{
    Enter,
    UpState,
    DownState,
    Exit,
    End,
};

class CDamageStarEffect : public CScript
{
private:
    DamageStarTickType m_eTickType;
    DamageStarSpawnType m_eSpawnType;
    DamageStarState m_eState;

    // Common Value
    bool m_bUseGravity;
    float m_fScaling;
    float m_fMaxScale;

    float m_fRadomMizeMinScale;
    float m_fRadomMizeMaxScale;

    float m_fRandomMizeMinPower;
    float m_fRandomMizeMaxPower;

    // Static Value
    Vec3 m_vDir;
    float m_fPower;

    // Velocity Value
    float m_fThreshHoldValue;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(DamageStarState _state) { m_eState = _state; }

    void Enter();
    void UpState();
    void DownState();
    void Exit();
    void TrackCamera();

    void Scaling(bool _bFlag);
    Vec3 RadomizeDir();
    float RandomizePow();
    float RandomizeScale();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CDamageStarEffect)
public:
    CDamageStarEffect();
    CDamageStarEffect(const CDamageStarEffect& Origin);
    virtual ~CDamageStarEffect();
};
