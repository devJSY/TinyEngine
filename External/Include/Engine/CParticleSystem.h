#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdate.h"

class CStructuredBuffer;

class CParticleSystem : public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_ModuleBuffer;
    CStructuredBuffer* m_RWBuffer;

    UINT m_MaxParticleCount;
    tParticleModule m_Module;

    Ptr<CParticleUpdate> m_CSParticleUpdate;
    Ptr<CTexture> m_ParticleTex;

    float m_AccTime;

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;
    virtual void render(UINT _Subset) override;

public:
    void EnableModule(PARTICLE_MODULE _ModuleType, bool _bEnable) { m_Module.arrModuleCheck[(UINT)_ModuleType] = _bEnable; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CParticleSystem);

public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& origin);
    virtual ~CParticleSystem();

    friend class COutliner;
};
