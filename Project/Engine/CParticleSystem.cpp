#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CTimeMgr.h"

#include "CAssetMgr.h"
#include "CMesh.h"
#include "CMaterial.h"

#include "components.h"

CParticleSystem::CParticleSystem()
    : CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
    , m_ParticleBuffer(nullptr)
    , m_MaxParticleCount(1000)
    , m_Module{}
    , m_ModuleBuffer(nullptr)
    , m_RWBuffer(nullptr)
    , m_CSParticleUpdate()
    , m_AccTime(0)
{
    // 전용 메쉬와 전용 재질 사용
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl"));

    // 렌더링 해상도
    Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

    // 임시로 5개의 파티클이 초기 데이터를 입력하면서 구조화 버퍼 생성
    tParticle arrParticle[1000] = {};
    for (int i = 0; i < m_MaxParticleCount; ++i)
    {
        arrParticle[i].vWorldPos = Vec3((vResol.x / -2.f) + (i + 1) * vResol.x / (m_MaxParticleCount + 1), 0.f, 200.f);
        arrParticle[i].vWorldScale = Vec3(10.f, 10.f, 1.f);
        arrParticle[i].Active = 0;
    }

    // 파티클을 저장하는 구조화 버퍼
    m_ParticleBuffer = new CStructuredBuffer;
    m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true, arrParticle);

    // 파티클 모듈정보를 저장하는 구조화버퍼
    m_ModuleBuffer = new CStructuredBuffer;
    m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);

    // 파티클 업데이트용 컴퓨트 쉐이더 참조
    m_CSParticleUpdate =
        (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleUpdateShader").Get();

    // 데이터 읽기/쓰기 용 구조화버퍼
    m_RWBuffer = new CStructuredBuffer;
    m_RWBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::READ_WRITE, true);
}

CParticleSystem::~CParticleSystem()
{
    if (nullptr != m_ParticleBuffer)
        delete m_ParticleBuffer;

    if (nullptr != m_ModuleBuffer)
        delete m_ModuleBuffer;

    if (nullptr != m_RWBuffer)
        delete m_RWBuffer;
}

void CParticleSystem::finaltick()
{
    // 파티클 모듈값 세팅
    m_Module.SpaceType = 1;
    m_Module.vSpawnColor = Vec4(1.f, 0.f, 0.f, 1.f);
    m_Module.vSpawnMinScale = Vec4(20.f, 20.f, 1.f, 1.f);
    m_Module.vSpawnMaxScale = Vec4(20.f, 20.f, 1.f, 1.f);
    m_Module.MinLife = 5.f;
    m_Module.MaxLife = 5.f;
    m_Module.SpawnRate = 100;

    m_AccTime += DT;
    if ((1.f / m_Module.SpawnRate) < m_AccTime)
    {
        m_AccTime = 0.f;
        tSpawnCount count = tSpawnCount{
            100,
        };
        m_RWBuffer->SetData(&count);
    }
    else
    {
        tSpawnCount count = tSpawnCount{
            0,
        };
        m_RWBuffer->SetData(&count);
    }

    // 파티클 모듈정보 업데이트
    m_ModuleBuffer->SetData(&m_Module);
    m_ModuleBuffer->UpdateData_CS_SRV(20);

    // 파티클 업데이트 컴퓨트 쉐이더
    m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
    m_CSParticleUpdate->SetModuleBuffer(m_ModuleBuffer);
    m_CSParticleUpdate->SetRWBuffer(m_RWBuffer);

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