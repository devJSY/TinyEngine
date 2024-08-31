#pragma once
#include <Engine\\CScript.h>

class CDestroyParticleScript : public CScript
{
private:
    float m_SpawnTime;
    float m_MaxLifeTime;
    float m_AccTime;
    bool m_bLive;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetSpawnTime(float _Time) { m_SpawnTime = _Time; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CDestroyParticleScript);
    CDestroyParticleScript();
    CDestroyParticleScript(const CDestroyParticleScript& _Origin);
    virtual ~CDestroyParticleScript();
};
