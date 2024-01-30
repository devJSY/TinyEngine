#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdate.h"

class CStructuredBuffer;

class CParticleSystem : public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    UINT m_MaxParticleCount;

    tParticleModule m_Module;
    CStructuredBuffer* m_ModuleBuffer;

    CStructuredBuffer* m_RWBuffer;

    Ptr<CParticleUpdate> m_CSParticleUpdate;

    Ptr<CTexture> m_ParticleTex;

    float m_AccTime;

public:
    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

public:
    CParticleSystem();
    virtual ~CParticleSystem();

    friend class COutliner;
};
