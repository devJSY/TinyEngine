#include "pch.h"
#include "CLandScape.h"

#include "CAssetMgr.h"
#include "CStructuredBuffer.h"

void CLandScape::Init()
{
    CreateMesh();

    // 지형 전용 재질 적용
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"), 0);

    // 지형 전용 컴퓨트 쉐이더 생성
    CreateComputeShader();

    // 지형 전용 텍스쳐 생성
    CreateTexture();

    // 레이캐스팅 결과 받는 버퍼
    m_CrossBuffer = new CStructuredBuffer;
    m_CrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);

    // 타일 텍스쳐(Color, Normal 혼합, 총 6장)
    // m_TileArrTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_ARRR.dds", L"texture\\tile\\TILE_ARRR.dds");
    // m_TileArrTex = CAssetMgr::GetInst()->LoadTexture(L"texture\\tile\\TILE_ARRR.dds", L"texture\\tile\\TILE_ARRR.dds", 8);
    m_TileArrTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\tile\\TILE_ARRR.dds");
    if (nullptr != m_TileArrTex)
    {
        m_TileArrTex->GenerateMip(8);
    }
}

void CLandScape::CreateMesh()
{
    // 지형 전용 메쉬 생성 및 적용
    vector<Vtx> vecVtx;
    Vtx v;

    // 정점 배치
    for (UINT i = 0; i < m_FaceZ + 1; ++i)
    {
        for (UINT j = 0; j < m_FaceX + 1; ++j)
        {
            v.vPos = Vec3((float)j, 0.f, (float)i);
            v.vTangent = Vec3(1.f, 0.f, 0.f);
            v.vNormal = Vec3(0.f, 1.f, 0.f);

            v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
            v.vUV0 = Vec2((float)j, m_FaceZ - (float)i);

            vecVtx.push_back(v);
        }
    }

    // 인덱스 지정
    vector<UINT> vecIdx;

    for (UINT i = 0; i < m_FaceZ; ++i)
    {
        for (UINT j = 0; j < m_FaceX; ++j)
        {
            // 0
            // | \
			// 2--1
            vecIdx.push_back((m_FaceX + 1) * (i + 1) + j);
            vecIdx.push_back((m_FaceX + 1) * i + (j + 1));
            vecIdx.push_back((m_FaceX + 1) * i + j);

            // 1--2
            //  \ |
            //    0
            vecIdx.push_back((m_FaceX + 1) * i + (j + 1));
            vecIdx.push_back((m_FaceX + 1) * (i + 1) + j);
            vecIdx.push_back((m_FaceX + 1) * (i + 1) + j + 1);
        }
    }

    Ptr<CMesh> pMesh = new CMesh;
    pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
    SetMesh(pMesh);

    // 지형 전용 재질 적용
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"), 0);
}

void CLandScape::CreateComputeShader()
{
    // ======================
    // 높이 수정 컴퓨트 쉐이더
    // ======================
    m_CSHeightMap = (CHeightMapShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"HeightMapShader").Get();

    // =====================
    // 지형 피킹 컴퓨트 쉐이더
    // =====================
    m_CSRaycast = (CRaycastShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"RaycastShader").Get();

    // =======================
    // 가중치 수정 컴퓨트 쉐이더
    // =======================
    m_CSWeightMap = (CWeightMapShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"WeightMapShader").Get();
}

void CLandScape::CreateTexture()
{
    // 높이맵 텍스춰
    m_HeightMapTex = CAssetMgr::GetInst()->CreateTexture(L"LandScapeHeightMapTex", 2048, 2048, DXGI_FORMAT_R32_FLOAT,
                                                         D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, D3D11_USAGE_DEFAULT);

    // CLandScape에서만 높이맵 텍스춰를 관리하도록 에셋매니저에서 삭제
    GamePlayStatic::DeleteAsset(ASSET_TYPE::TEXTURE, m_HeightMapTex.Get());

    m_BrushTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\brush\\Brush_02.png");

    // 가중치 버퍼
    m_WeightWidth = 1024;
    m_WeightHeight = 1024;

    m_WeightMapBuffer = new CStructuredBuffer;
    m_WeightMapBuffer->Create(sizeof(tWeight_4), m_WeightWidth * m_WeightHeight, SB_TYPE::READ_WRITE, false);
}
