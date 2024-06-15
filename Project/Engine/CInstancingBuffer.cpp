#include "pch.h"
#include "CInstancingBuffer.h"

#include "CDevice.h"
#include "CMaterial.h"
#include "CAssetMgr.h"

CInstancingBuffer::CInstancingBuffer()
    : m_InstancingBuffer(nullptr)
    , m_MaxCount(10)
    , m_vecData{}
    , m_vecBoneMat{}
    , m_AnimInstCount(0)
    , m_BoneBuffer(nullptr)
    , m_CopyShader(nullptr)
{
    m_BoneBuffer = new CStructuredBuffer;
}

CInstancingBuffer::~CInstancingBuffer()
{
    if (nullptr != m_BoneBuffer)
    {
        delete m_BoneBuffer;
        m_BoneBuffer = nullptr;
    }
}

void CInstancingBuffer::init()
{
    D3D11_BUFFER_DESC tDesc = {};

    tDesc.ByteWidth = sizeof(tInstancingData) * m_MaxCount;
    tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    tDesc.Usage = D3D11_USAGE_DYNAMIC;
    tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_InstancingBuffer)))
        assert(NULL);

    m_CopyShader = (CCopyBoneShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"CopyBoneCS").Get();
}

void CInstancingBuffer::SetData()
{
    if (m_vecData.size() > m_MaxCount)
    {
        Resize((UINT)m_vecData.size());
    }

    D3D11_MAPPED_SUBRESOURCE tMap = {};

    CONTEXT->Map(m_InstancingBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
    memcpy(tMap.pData, &m_vecData[0], sizeof(tInstancingData) * m_vecData.size());
    CONTEXT->Unmap(m_InstancingBuffer.Get(), 0);

    // 본 행렬정보 메모리 복사
    if (m_vecBoneMat.empty())
        return;

    UINT iBufferSize = (UINT)m_vecBoneMat.size() * m_vecBoneMat[0]->GetBufferSize();
    if (m_BoneBuffer->GetBufferSize() < iBufferSize)
    {
        m_BoneBuffer->Create(m_vecBoneMat[0]->GetElementSize(), m_vecBoneMat[0]->GetElementCount() * (UINT)m_vecBoneMat.size(), SB_TYPE::READ_WRITE,
                             false, nullptr);
    }

    // 복사용 컴퓨트 쉐이더 실행
    UINT iBoneCount = m_vecBoneMat[0]->GetElementCount();
    m_CopyShader->SetBoneCount(iBoneCount);

    for (UINT i = 0; i < (UINT)m_vecBoneMat.size(); ++i)
    {
        m_CopyShader->SetRowIndex(i);
        m_CopyShader->SetSourceBuffer(m_vecBoneMat[i]);
        m_CopyShader->SetDestBuffer(m_BoneBuffer);
        m_CopyShader->Execute();
    }

    // Bone 정보 전달 레지스터
    m_BoneBuffer->UpdateData(31);
}

void CInstancingBuffer::AddInstancingBoneMat(CStructuredBuffer* _pBuffer)
{
    ++m_AnimInstCount;
    m_vecBoneMat.push_back(_pBuffer);
}

void CInstancingBuffer::Resize(UINT _iCount)
{
    m_InstancingBuffer = nullptr;

    m_MaxCount = _iCount;

    D3D11_BUFFER_DESC tDesc = {};

    tDesc.ByteWidth = sizeof(tInstancingData) * m_MaxCount;
    tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    tDesc.Usage = D3D11_USAGE_DYNAMIC;
    tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_InstancingBuffer)))
        assert(NULL);
}