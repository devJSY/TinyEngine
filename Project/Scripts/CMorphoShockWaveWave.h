#pragma once
#include <Engine\\CScript.h>

class CMorphoShockWaveWave : public CScript
{
private:
    Vec3 m_OriginScale;
    float m_AccTime;
    float m_PlayTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CMorphoShockWaveWave);
    CMorphoShockWaveWave();
    virtual ~CMorphoShockWaveWave();
};
