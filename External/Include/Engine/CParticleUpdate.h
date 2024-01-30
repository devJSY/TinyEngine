#pragma once
#include "CComputeShader.h"

class CParticleUpdate : public CComputeShader
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_ModuleBuffer;
    CStructuredBuffer* m_RWBuffer;
    Vec3 m_vParticleWorldPos;

public:
    void SetParticleBuffer(CStructuredBuffer* _ParticleBuffer) { m_ParticleBuffer = _ParticleBuffer; }
    void SetModuleBuffer(CStructuredBuffer* _Buffer) { m_ModuleBuffer = _Buffer; }
    void SetRWBuffer(CStructuredBuffer* _Buffer) { m_RWBuffer = _Buffer; }
    void SetParticleWorldPos(Vec3 _vWorldPos) { m_vParticleWorldPos = _vWorldPos; };

public:
    virtual int UpdateData() override;
    virtual void UpdateGroupCount() override;
    virtual void Clear() override;

public:
    CParticleUpdate();
    virtual ~CParticleUpdate();
};
