#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdate.h"

class CStructuredBuffer;

class CParticleSystem : public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    UINT m_MaxParticleCount;

    Ptr<CParticleUpdate> m_CSParticleUpdate;

public:
    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

public:
    CParticleSystem();
    virtual ~CParticleSystem();
};
