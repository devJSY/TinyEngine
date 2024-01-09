#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
    : m_ElementSize(0)
    , m_ElementCount(0)
    , m_Type(SB_TYPE::READ_ONLY)
    , m_bSysMemMove(false)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove, void* _pSysMem)
{
    // 구조화버퍼 크기가 16의 배수인지 체크
    assert(!(_ElementSize % 16));

    m_SB = nullptr;
    m_SRV = nullptr;
    m_SB_Read = nullptr;
    m_SB_Write = nullptr;

    m_ElementSize = _ElementSize;
    m_ElementCount = _ElementCount;
    m_Type = _Type;
    m_bSysMemMove = _bSysMemMove;

    D3D11_BUFFER_DESC tDesc = {};
    tDesc.ByteWidth = m_ElementSize * m_ElementCount;
    tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    tDesc.StructureByteStride = m_ElementSize;

    tDesc.CPUAccessFlags = 0;
    tDesc.Usage = D3D11_USAGE_DEFAULT;

    HRESULT hr = E_FAIL;
    if (nullptr == _pSysMem)
    {
        hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB.GetAddressOf());
    }
    else
    {
        D3D11_SUBRESOURCE_DATA tSub = {};
        tSub.pSysMem = _pSysMem;
        hr = DEVICE->CreateBuffer(&tDesc, &tSub, m_SB.GetAddressOf());
    }

    if (FAILED(hr))
        return E_FAIL;

    // Shader Resource View 생성
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    SRVDesc.Buffer.NumElements = m_ElementCount;

    hr = DEVICE->CreateShaderResourceView(m_SB.Get(), &SRVDesc, m_SRV.GetAddressOf());
    if (FAILED(hr))
        return E_FAIL;

    if (m_bSysMemMove)
    {
        // 쓰기용 버퍼
        tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        tDesc.Usage = D3D11_USAGE_DYNAMIC;
        hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB_Write.GetAddressOf());

        // 읽기용 버퍼
        tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        tDesc.Usage = D3D11_USAGE_DEFAULT;
        hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB_Read.GetAddressOf());
    }

    return S_OK;
}

void CStructuredBuffer::UpdateData(UINT _RegisterNum)
{
    CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
    CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::SetData(void* _SysMem, UINT _ElementCount)
{
    if (nullptr == _SysMem)
        return;	

    assert(m_bSysMemMove);

    if (0 == _ElementCount)
        _ElementCount = m_ElementCount;

    // 입력 데이터가 구조화버퍼보다 더 큰 경우
    if (m_ElementCount < _ElementCount)
    {
        Create(m_ElementSize, _ElementCount, m_Type, m_bSysMemMove, nullptr);
    }

    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
    memcpy(tSub.pData, _SysMem, m_ElementSize * _ElementCount);
    CONTEXT->Unmap(m_SB_Write.Get(), 0);

    // Write Buffer -> Main Buffer
    CONTEXT->CopyResource(m_SB.Get(), m_SB_Write.Get());
}

void CStructuredBuffer::GetData(void* _Dest, UINT _ElementCount)
{
    assert(m_bSysMemMove);

    if (0 == _ElementCount)
        _ElementCount = m_ElementCount;

    // Min Buffer -> Read Buffer
    CONTEXT->CopyResource(m_SB_Read.Get(), m_SB.Get());

    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP_READ, 0, &tSub);
    memcpy(_Dest, tSub.pData, m_ElementSize * _ElementCount);
    CONTEXT->Unmap(m_SB_Read.Get(), 0);
}
