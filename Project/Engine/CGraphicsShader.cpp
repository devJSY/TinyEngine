#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

CGraphicsShader::CGraphicsShader()
    : CShader(ASSET_TYPE::GRAPHICS_SHADER)
    , m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CGraphicsShader::~CGraphicsShader()
{
}

int CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)
{
    // 버텍스 쉐이더
    // HLSL 버텍스 쉐이더 함수 컴파일
    wstring strContentPath = CPathMgr::GetContentPath();
    wstring strFilePath = strContentPath + _strRelativePath;

    if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(),
                                  "vs_5_0", D3DCOMPILE_DEBUG, 0, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
    {
        if (nullptr != m_ErrBlob)
        {
            char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
            MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
        }

        return E_FAIL;
    }

    DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());

    // 정점 구조정보(Layout) 생성
    D3D11_INPUT_ELEMENT_DESC arrElement[3] = {}; // 정점하나안의 요소 갯수만큼 생성

    arrElement[0].InputSlot = 0;
    arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    arrElement[0].SemanticName = "POSITION";
    arrElement[0].SemanticIndex = 0;
    arrElement[0].InstanceDataStepRate = 0;
    arrElement[0].AlignedByteOffset = 0;
    arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

    arrElement[1].InputSlot = 0;
    arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    arrElement[1].SemanticName = "COLOR";
    arrElement[1].SemanticIndex = 0;
    arrElement[1].InstanceDataStepRate = 0;
    arrElement[1].AlignedByteOffset = 12;
    arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    arrElement[2].InputSlot = 0;
    arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    arrElement[2].SemanticName = "TEXCOORD";
    arrElement[2].SemanticIndex = 0;
    arrElement[2].InstanceDataStepRate = 0;
    arrElement[2].AlignedByteOffset = 28;
    arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

    DEVICE->CreateInputLayout(arrElement, 3, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
                              m_Layout.GetAddressOf());

    return S_OK;
}

int CGraphicsShader::CreateHullShader()
{
    return S_OK;
}

int CGraphicsShader::CreateDomainShader()
{
    return S_OK;
}

int CGraphicsShader::CreateGeometryShader()
{
    return S_OK;
}

int CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName)
{
    wstring strContentPath = CPathMgr::GetContentPath();
    wstring strFilePath = strContentPath + _strRelativePath;

    // 픽셀 쉐이더 생성
    // 픽셀 쉐이더 컴파일
    if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(),
                                  "ps_5_0", D3DCOMPILE_DEBUG, 0, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
    {
        if (nullptr != m_ErrBlob)
        {
            char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
            MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
        }

        return E_FAIL;
    }

    DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

    return S_OK;
}

void CGraphicsShader::UpdateData()
{
    CONTEXT->IASetInputLayout(m_Layout.Get());
    CONTEXT->IASetPrimitiveTopology(m_Topology);

    CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
    CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
    CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
    CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
    CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);
}
