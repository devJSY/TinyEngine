#pragma once
#include "CComputeShader.h"

class CParticleUpdate : public CComputeShader
{
private:
    CStructuredBuffer* m_ParticleBuffer;

public:
    void SetParticleBuffer(CStructuredBuffer* _ParticleBuffer) { m_ParticleBuffer = _ParticleBuffer; }

public:
    virtual int UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    CParticleUpdate();
    virtual ~CParticleUpdate();
};
