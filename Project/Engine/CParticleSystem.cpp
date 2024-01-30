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
    , m_MaxParticleCount(2000)
    , m_Module{}
    , m_ModuleBuffer(nullptr)
    , m_RWBuffer(nullptr)
    , m_CSParticleUpdate()
    , m_ParticleTex()
    , m_AccTime(0)
{
    // 전용 메쉬와 전용 재질 사용
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl"));

    // 렌더링 해상도
    Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

    // 파티클을 저장하는 구조화 버퍼
    m_ParticleBuffer = new CStructuredBuffer;
    m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true);

    // 파티클 모듈정보를 저장하는 구조화버퍼
    m_ModuleBuffer = new CStructuredBuffer;
    m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);

    // 파티클 업데이트용 컴퓨트 쉐이더 참조
    m_CSParticleUpdate =
        (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleUpdateShader").Get();

    // 데이터 읽기/쓰기 용 구조화버퍼
    m_RWBuffer = new CStructuredBuffer;
    m_RWBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::READ_WRITE, true);

    // 초기 모듈 세팅
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

    m_Module.SpaceType = 1;
    m_Module.vSpawnColor = Vec4(0.2f, 0.4f, 0.9f, 1.f);
    m_Module.vSpawnMinScale = Vec4(50.f, 50.f, 1.f, 1.f);
    m_Module.vSpawnMaxScale = Vec4(200.f, 200.f, 1.f, 1.f);
    m_Module.MinLife = 0.4f;
    m_Module.MaxLife = 1.f;
    m_Module.SpawnShape = 1; // 0 : Sphere, 1 : Box
    m_Module.Radius = 100.f;
    m_Module.vSpawnBoxScale = Vec4(500.f, 500.f, 0.f, 0.f);
    m_Module.SpawnRate = 50;

    // Add Velocity Module
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
    m_Module.AddVelocityType = 0;
    m_Module.MinSpeed = 100;
    m_Module.MaxSpeed = 200;
    m_Module.FixedDirection;
    m_Module.FixedAngle;

    m_ParticleTex = CAssetMgr::GetInst()->Load<CTexture>(L"Textures\\particle\\Bubbles50px.png",
                                                         L"Textures\\particle\\Bubbles50px.png");
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
    m_AccTime += DT;

    if ((1.f / m_Module.SpawnRate) < m_AccTime)
    {
        // 누적 시간을 스폰 간격으로 나눈 값
        float fSpawnCount = m_AccTime / (1.f / m_Module.SpawnRate);

        // 스폰 간격을 제외한 잔량을 남은 누적시간으로 설정
        m_AccTime -= (1.f / m_Module.SpawnRate) * floorf(fSpawnCount);

        tSpawnCount count = tSpawnCount{(int)fSpawnCount, 0, 0, 0};
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
    m_CSParticleUpdate->SetParticleWorldPos(Transform()->GetWorldPos());

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
    GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
    GetMesh()->render_particle(m_MaxParticleCount);

    // 렌더링때 사용한 리소스 바인딩 Clear
    m_ParticleBuffer->Clear(20);
}

void CParticleSystem::UpdateData()
{
}