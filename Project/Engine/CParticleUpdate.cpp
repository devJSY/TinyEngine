#include "pch.h"
#include "CParticleUpdate.h"

CParticleUpdate::CParticleUpdate()
    : CComputeShader(32, 1, 1)
    , m_ParticleBuffer(nullptr)
    , m_ModuleBuffer(nullptr)
    , m_RWBuffer(nullptr)

{
    Create(L"shader\\particle_update.fx", "CS_ParticleUpdate");
}

CParticleUpdate::~CParticleUpdate()
{
}

int CParticleUpdate::UpdateData()
{
    if (nullptr == m_ParticleBuffer)
        return E_FAIL;

    m_Const.arrInt[0] = m_ParticleBuffer->GetElementCount();

    m_ParticleBuffer->UpdateData_CS_UAV(0);
    m_RWBuffer->UpdateData_CS_UAV(1);

    m_ModuleBuffer->UpdateData_CS_SRV(20);

    return S_OK;
}

void CParticleUpdate::UpdateGroupCount()
{
    UINT GroupX = (m_ParticleBuffer->GetElementCount() / m_ThreadX);
    if (0 != m_ParticleBuffer->GetElementCount() % m_ThreadX)
        GroupX += 1;

    SetGroup(GroupX, 1, 1);
}

void CParticleUpdate::Clear()
{
    m_ParticleBuffer->Clear_CS_UAV();
    m_ParticleBuffer = nullptr;

    m_ModuleBuffer->Clear_CS_SRV();
    m_ModuleBuffer = nullptr;

    m_RWBuffer->Clear_CS_UAV();
    m_RWBuffer = nullptr;
}