#include "pch.h"
#include "CLandScape.h"

#include "CAssetMgr.h"

void CLandScape::Init()
{
    CreateMesh();
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
            v.vUV = Vec2((float)j, float(m_FaceZ - i));

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
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"));
}