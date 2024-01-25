#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CAssetMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "components.h"

CParticleSystem::CParticleSystem()
    : CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
    , m_ParticleBuffer(nullptr)
    , m_MaxParticleCount(100)
{
    // 전용 메쉬와 전용 재질 사용
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl"));

    // 렌더링 해상도
    Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

    // 임시로 5개의 파티클이 초기 데이터를 입력하면서 구조화 버퍼 생성
    tParticle arrParticle[100] = {};
    for (UINT i = 0; i < m_MaxParticleCount; ++i)
    {
        arrParticle[i].vWorldPos = Vec3((vResol.x / -2.f) + (i + 1) * vResol.x / (m_MaxParticleCount + 1), 0.f, 200.f);
        arrParticle[i].vWorldScale = Vec3(10.f, 10.f, 1.f);
        arrParticle[i].Active = 1;
    }

    m_ParticleBuffer = new CStructuredBuffer;
    m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true, arrParticle);

    // 파티클 업데이트용 컴퓨트 쉐이더 참조
    m_CSParticleUpdate =
        (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleUpdateShader").Get();
}

CParticleSystem::~CParticleSystem()
{
    if (nullptr != m_ParticleBuffer)
        delete m_ParticleBuffer;
}

void CParticleSystem::finaltick()
{
    // 파티컬 업데이트 컴퓨트 쉐이더
    m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
    m_CSParticleUpdate->Execute();
}

void CParticleSystem::render()
{
    // View, Proj 행렬 전달
    Transform()->UpdateData();

    // ParticleBuffer 바인딩
    m_ParticleBuffer->UpdateData(20);

    // 모든 파티클 렌더링
    // 파티클 개별 랜더링 -> 인스턴싱
    GetMaterial()->UpdateData();
    GetMesh()->render_particle(m_MaxParticleCount);

    // 렌더링때 사용한 리소스 바인딩 Clear
    m_ParticleBuffer->Clear(20);
}

void CParticleSystem::UpdateData()
{
}