#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

class CParticleSystem : public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    UINT m_MaxParticleCount;

public:
    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

public:
    CParticleSystem();
    virtual ~CParticleSystem();
};
