#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

#include "CAssetMgr.h"
#include "CTexture.h"

CMesh::CMesh(bool _bEngineAsset)
    : CAsset(ASSET_TYPE::MESH, _bEngineAsset)
    , m_VBDesc{}
    , m_IBDesc{}
    , m_VtxCount(0)
    , m_IdxCount(0)
    , m_VtxSysMem(nullptr)
    , m_IdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
    if (nullptr != m_VtxSysMem)
    {
        delete m_VtxSysMem;
        m_VtxSysMem = nullptr;
    }

    if (nullptr != m_IdxSysMem)
    {
        delete m_IdxSysMem;
        m_IdxSysMem = nullptr;
    }
}

int CMesh::Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount)
{
    m_VtxCount = _VtxCount;
    m_IdxCount = _IdxCount;

    // 버텍스 버퍼 생성;
    m_VBDesc = {};

    m_VBDesc.ByteWidth = sizeof(Vtx) * m_VtxCount;
    m_VBDesc.StructureByteStride = sizeof(Vtx); // 버텍스 1개의 크기
    m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    m_VBDesc.CPUAccessFlags = 0;
    m_VBDesc.Usage = D3D11_USAGE_DEFAULT;

    // 입력 배열의 데이터를 초기 데이터로 설정
    D3D11_SUBRESOURCE_DATA tSubData = {};
    tSubData.pSysMem = _Vtx;

    // 버텍스 버퍼 생성
    if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSubData, m_VB.GetAddressOf())))
    {
        MessageBox(nullptr, L"버텍스 버퍼 생성 실패", L"오류", MB_OK);
        return E_FAIL;
    }

    m_IBDesc = {};

    m_IBDesc.ByteWidth = sizeof(UINT) * m_IdxCount;
    m_IBDesc.StructureByteStride = sizeof(UINT);
    m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    m_IBDesc.CPUAccessFlags = 0;
    m_IBDesc.Usage = D3D11_USAGE_DEFAULT;

    tSubData = {};
    tSubData.pSysMem = _Idx;

    // 인덱스 버퍼 생성
    if (FAILED(DEVICE->CreateBuffer(&m_IBDesc, &tSubData, m_IB.GetAddressOf())))
    {
        MessageBox(nullptr, L"인덱스 버퍼 생성 실패", L"오류", MB_OK);
        return E_FAIL;
    }

    // 원본 정점정보 및 인덱스 정보를 동적할당한 곳에다가 저장시켜두고 관리

    m_VtxSysMem = new Vtx[m_VtxCount];
    m_IdxSysMem = new UINT[m_IdxCount];

    memcpy(m_VtxSysMem, _Vtx, sizeof(Vtx) * m_VtxCount);
    memcpy(m_IdxSysMem, _Idx, sizeof(UINT) * m_IdxCount);

    return S_OK;
}

void CMesh::UpdateData()
{
    UINT iStride = sizeof(Vtx);
    UINT iOffset = 0;
    CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
    CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render()
{
    UpdateData();
    CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

void CMesh::render_draw()
{
    UpdateData();
    CONTEXT->Draw(m_VtxCount, 0);
}

void CMesh::render_IndexedInstanced(UINT _InstanceCount)
{
    UpdateData();
    CONTEXT->DrawIndexedInstanced(m_IdxCount, _InstanceCount, 0, 0, 0);
}
