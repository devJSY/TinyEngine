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
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl"), 0);

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
    m_CSParticleUpdate = (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleUpdateShader").Get();

    // 모듈 초기화
    // Spawn 모듈
    m_Module.vSpawnColor = Vec4(0.f, 0.f, 0.f, 1.f);
    m_Module.vSpawnMinScale = Vec4(50.f, 50.f, 1.f, 1.f);
    m_Module.vSpawnMaxScale = Vec4(50.f, 50.f, 1.f, 1.f);

    m_Module.MinLife = 5.f;
    m_Module.MaxLife = 5.f;

    m_Module.MinMass = 1.f;
    m_Module.MaxMass = 1.f;

    m_Module.SpawnRate = 10;
    m_Module.SpaceType = 0; // Local Space

    m_Module.SpawnShape = 0; // Sphere
    m_Module.Radius = 1.f;
    m_Module.vSpawnBoxScale = Vec4(1.f, 1.f, 1.f, 1.f);

    // Scale 모듈
    m_Module.vScaleRatio = Vec4(1.f, 1.f, 1.f, 1.f);

    // Add Velocity 모듈
    m_Module.AddVelocityType = 0; // From Center
    m_Module.MinSpeed = 100.f;
    m_Module.MaxSpeed = 100.f;
    m_Module.FixedAngle = 180.f;
    m_Module.vFixedDirection = Vec4(0.f, 0.f, 0.f, 1.f);

    // Noise Force 모듈
    m_Module.NoiseForceScale = 100.f;
    m_Module.NoiseForceTerm = 1.f;

    // Render 모듈
    m_Module.VelocityAlignment = 0; // Off
    m_Module.AlphaBasedLife = 0;    // Off
    m_Module.AlphaMaxAge = 1.f;

    SetFrustumCheck(false);
    SetCastShadow(false);
}

CParticleSystem::CParticleSystem(const CParticleSystem& origin)
    : CRenderComponent(origin)
    , m_ParticleBuffer(nullptr)
    , m_ModuleBuffer(nullptr)
    , m_RWBuffer(nullptr)
    , m_MaxParticleCount(origin.m_MaxParticleCount)
    , m_Module(origin.m_Module)
    , m_CSParticleUpdate(origin.m_CSParticleUpdate)
    , m_ParticleTex(origin.m_ParticleTex)
    , m_AccTime(origin.m_AccTime)
{
    if (nullptr != origin.m_ParticleBuffer)
        m_ParticleBuffer = origin.m_ParticleBuffer->Clone();

    if (nullptr != origin.m_ModuleBuffer)
        m_ModuleBuffer = origin.m_ModuleBuffer->Clone();

    if (nullptr != origin.m_RWBuffer)
        m_RWBuffer = origin.m_RWBuffer->Clone();
}

CParticleSystem::~CParticleSystem()
{
    if (nullptr != m_ParticleBuffer)
    {
        delete m_ParticleBuffer;
        m_ParticleBuffer = nullptr;
    }

    if (nullptr != m_ModuleBuffer)
    {
        delete m_ModuleBuffer;
        m_ModuleBuffer = nullptr;
    }

    if (nullptr != m_RWBuffer)
    {
        delete m_RWBuffer;
        m_RWBuffer = nullptr;
    }
}

void CParticleSystem::finaltick()
{
    CRenderComponent::finaltick();

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
    m_ModuleBuffer->UpdateData(21);

    // 모든 파티클 렌더링
    // 파티클 개별 랜더링 -> 인스턴싱
    GetMaterial(0)->UpdateData();
    GetMaterial(0)->SetTexParam(TEX_0, m_ParticleTex);

    // Animatio2D 보유한 경우
    if (Animator2D())
        Animator2D()->UpdateData();
}

void CParticleSystem::render()
{
    UpdateData();

    GetMesh()->render_IndexedInstanced(m_MaxParticleCount);

    // 렌더링때 사용한 리소스 바인딩 Clear
    m_ParticleBuffer->Clear(20);
    m_ModuleBuffer->Clear(21);

    // Animation 관련 정보 제거
    if (Animator2D())
        Animator2D()->Clear();
}

void CParticleSystem::render(Ptr<CMaterial> _mtrl)
{
    // View, Proj 행렬 전달
    Transform()->UpdateData();

    // ParticleBuffer 바인딩
    m_ParticleBuffer->UpdateData(20);
    m_ModuleBuffer->UpdateData(21);

    // 모든 파티클 렌더링
    // 파티클 개별 랜더링 -> 인스턴싱
    _mtrl->UpdateData();
    _mtrl->SetTexParam(TEX_0, m_ParticleTex);

    GetMesh()->render_IndexedInstanced(m_MaxParticleCount);

    // 렌더링때 사용한 리소스 바인딩 Clear
    m_ParticleBuffer->Clear(20);
    m_ModuleBuffer->Clear(21);

    // Animation 관련 정보 제거
    if (Animator2D())
        Animator2D()->Clear();
}

void CParticleSystem::render(UINT _Subset)
{
    render();
}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
    CRenderComponent::SaveToLevelFile(_File);

    fwrite(&m_MaxParticleCount, sizeof(UINT), 1, _File);
    fwrite(&m_Module, sizeof(tParticleModule), 1, _File);

    SaveAssetRef<CComputeShader>(m_CSParticleUpdate.Get(), _File);
    SaveAssetRef<CTexture>(m_ParticleTex.Get(), _File);
}

void CParticleSystem::LoadFromLevelFile(FILE* _File)
{
    CRenderComponent::LoadFromLevelFile(_File);

    fread(&m_MaxParticleCount, sizeof(UINT), 1, _File);
    m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::READ_WRITE, true);
    fread(&m_Module, sizeof(tParticleModule), 1, _File);

    bool bAssetExist = 0;
    fread(&bAssetExist, sizeof(bAssetExist), 1, _File);

    if (bAssetExist)
    {
        wstring strKey, strRelativePath;
        LoadWStringFromFile(strKey, _File);
        LoadWStringFromFile(strRelativePath, _File);

        m_CSParticleUpdate = (CParticleUpdate*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(strKey).Get();
    }

    LoadAssetRef<CTexture>(m_ParticleTex, _File);
}
