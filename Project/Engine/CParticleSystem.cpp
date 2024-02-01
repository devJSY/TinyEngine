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
    , m_ModuleBuffer(nullptr)
    , m_RWBuffer(nullptr)
    , m_MaxParticleCount(1000)
    , m_Module{}
    , m_CSParticleUpdate()
    , m_ParticleTex()
    , m_AccTime(0)
{
    // 전용 메쉬와 전용 재질 사용
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl"));

    // 파티클을 저장하는 구조화 버퍼
    m_ParticleBuffer = new CStructuredBuffer;
    m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true);

    // 파티클 모듈정보를 저장하는 구조화버퍼
    m_ModuleBuffer = new CStructuredBuffer;
    m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);

    // 데이터 읽기/쓰기 용 구조화버퍼
    m_RWBuffer = new CStructuredBuffer;
    m_RWBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::READ_WRITE, true);

    // 파티클 업데이트용 컴퓨트 쉐이더 참조
    m_CSParticleUpdate =
        (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleUpdateShader").Get();

    // 초기 모듈 세팅
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

    m_Module.SpaceType = 1;
    m_Module.vSpawnColor = Vec4(0.2f, 0.4f, 0.9f, 1.f);
    m_Module.vSpawnMinScale = Vec4(100.f, 30.f, 1.f, 1.f);
    m_Module.vSpawnMaxScale = Vec4(100.f, 30.f, 1.f, 1.f);
    m_Module.MinLife = 5.f;
    m_Module.MaxLife = 5.f;
    m_Module.MinMass = 1.f;
    m_Module.MaxMass = 1.f;
    m_Module.SpawnShape = 1; // 0 : Sphere, 1 : Box
    m_Module.Radius = 100.f;
    m_Module.vSpawnBoxScale = Vec4(500.f, 500.f, 0.f, 0.f);
    m_Module.SpawnRate = 50;

    // Add Velocity Module
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 0;
    m_Module.AddVelocityType = 0;
    m_Module.MinSpeed = 100;
    m_Module.MaxSpeed = 150;
    m_Module.vFixedDirection;
    m_Module.FixedAngle;

    // Scale
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;
    m_Module.vScaleRatio = Vec3(2.f, 2.f, 2.f);

    // Noise Force
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
    m_Module.NoiseForceScale = 50.f;
    m_Module.NoiseForceTerm = 0.3f;

    // Render
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
    m_Module.VelocityAlignment = 1; // 속도에 따른 방향 정렬

    // Calculate Force
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;

    
	// 초기 모듈 세팅
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

    m_Module.SpaceType = 1;
    m_Module.vSpawnColor = Vec4(0.2f, 0.4f, 0.9f, 1.f);
    m_Module.vSpawnMinScale = Vec4(100.f, 30.f, 1.f, 1.f);
    m_Module.vSpawnMaxScale = Vec4(100.f, 30.f, 1.f, 1.f);
    m_Module.MinLife = 3.f;
    m_Module.MaxLife = 5.f;
    m_Module.MinMass = 1.f;
    m_Module.MaxMass = 1.f;
    m_Module.SpawnShape = 1; // 0 : Sphere, 1 : Box
    m_Module.Radius = 100.f;
    m_Module.vSpawnBoxScale = Vec4(500.f, 500.f, 0.f, 0.f);
    m_Module.SpawnRate = 50;

    // Add Velocity Module
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 0;
    m_Module.AddVelocityType = 0;
    m_Module.MinSpeed = 100;
    m_Module.MaxSpeed = 150;
    m_Module.vFixedDirection;
    m_Module.FixedAngle;

    // Scale
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;
    m_Module.vScaleRatio = Vec3(0.1f, 0.1f, 0.1f);

    // Noise Force
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 0;
    m_Module.NoiseForceScale = 50.f;
    m_Module.NoiseForceTerm = 0.3f;

    // Calculate Forec
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;

    // Render
    m_Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
    m_Module.VelocityAlignment = 1; // 속도에 따른 방향 정렬


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

        tSpawnCount count = {};
        count.SpawnCount = (int)fSpawnCount; 
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
    m_ModuleBuffer->UpdateData(21);

    // 파티클 업데이트 컴퓨트 쉐이더
    m_CSParticleUpdate->SetParticleBuffer(m_ParticleBuffer);
    m_CSParticleUpdate->SetModuleBuffer(m_ModuleBuffer);
    m_CSParticleUpdate->SetRWBuffer(m_RWBuffer);
    m_CSParticleUpdate->SetParticleWorldPos(Transform()->GetWorldPos());

    m_CSParticleUpdate->Execute();
}

void CParticleSystem::UpdateData()
{
    // View, Proj 행렬 전달
    Transform()->UpdateData();

    // ParticleBuffer 바인딩
    m_ParticleBuffer->UpdateData(20);

    // 모든 파티클 렌더링
    // 파티클 개별 랜더링 -> 인스턴싱
    GetMaterial()->UpdateData();
    GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
}

void CParticleSystem::render()
{
    UpdateData();

    GetMesh()->render_particle(m_MaxParticleCount);

    // 렌더링때 사용한 리소스 바인딩 Clear
    m_ParticleBuffer->Clear(20);
    m_ModuleBuffer->Clear(21);
}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
    CRenderComponent::SaveToLevelFile(_File);

    fwrite(&m_MaxParticleCount, sizeof(UINT), 1, _File);
    fwrite(&m_Module, sizeof(tParticleModule), 1, _File);

    SaveAssetRef(m_CSParticleUpdate.Get(), _File);
    SaveAssetRef(m_ParticleTex.Get(), _File);
}

void CParticleSystem::LoadFromLevelFile(FILE* _File)
{
    CRenderComponent::LoadFromLevelFile(_File);

    fread(&m_MaxParticleCount, sizeof(UINT), 1, _File);
    m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true);
    fread(&m_Module, sizeof(tParticleModule), 1, _File);

    int i = 0;
    fread(&i, sizeof(i), 1, _File);

    if (i)
    {
        wstring strKey, strRelativePath;
        LoadWString(strKey, _File);
        LoadWString(strRelativePath, _File);

        m_CSParticleUpdate = (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(strKey).Get();
    }

    LoadAssetRef<CTexture>(m_ParticleTex, _File);
}
