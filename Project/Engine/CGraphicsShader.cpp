#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

CGraphicsShader::CGraphicsShader()
    : CShader(ASSET_TYPE::GRAPHICS_SHADER)
    , m_VSBlob(nullptr)
    , m_VSInstBlob(nullptr)
    , m_HSBlob(nullptr)
    , m_DSBlob(nullptr)
    , m_GSBlob(nullptr)
    , m_PSBlob(nullptr)
    , m_VS(nullptr)
    , m_VSInst(nullptr)
    , m_HS(nullptr)
    , m_DS(nullptr)
    , m_GS(nullptr)
    , m_PS(nullptr)
    , m_Layout(nullptr)
    , m_LayoutInst(nullptr)
    , m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    , m_RSType(RS_TYPE::CULL_BACK)
    , m_DSType(DS_TYPE::LESS)
    , m_BSType(BS_TYPE::DEFAULT)
    , m_Domain(SHADER_DOMAIN::DOMAIN_MASKED)
    , m_ScalarParam{}
    , m_TexParam{}
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

    if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "vs_5_0", m_CompileFlags, 0,
                                  m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
    {
        if (nullptr != m_ErrBlob)
        {
            char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
            MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
        }
        else
        {
            MessageBoxA(nullptr, "Shader File No Exist", "Shader Compile Failed!!", MB_OK);
        }

        return E_FAIL;
    }

    DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());

    // Layout 생성
    const vector<D3D11_INPUT_ELEMENT_DESC>& vecLayout = CAssetMgr::GetInst()->GetInputLayoutInfo();

    DEVICE->CreateInputLayout(vecLayout.data(), (UINT)vecLayout.size(), m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
                              m_Layout.GetAddressOf());

    // Vertex Inst Shader 컴파일 하기
    HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, (_strFuncName + "_Inst").c_str(), "vs_5_0",
                                    m_CompileFlags, 0, m_VSInstBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

    if (SUCCEEDED(hr))
    {
        // 컴파일 된 코드로 Vertex Instancing Shader 객체 만들기
        DEVICE->CreateVertexShader(m_VSInstBlob->GetBufferPointer(), m_VSInstBlob->GetBufferSize(), nullptr, m_VSInst.GetAddressOf());

        DEVICE->CreateInputLayout(vecLayout.data(), (UINT)vecLayout.size(), m_VSInstBlob->GetBufferPointer(), m_VSInstBlob->GetBufferSize(),
                                  m_LayoutInst.GetAddressOf());
    }
    else
    {
        return E_FAIL;
    }

    return S_OK;
}

int CGraphicsShader::CreateHullShader(const wstring& _strRelativePath, const string& _strFuncName)
{
    wstring strContentPath = CPathMgr::GetContentPath();
    wstring strFilePath = strContentPath + _strRelativePath;

    if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "hs_5_0", m_CompileFlags, 0,
                                  m_HSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
    {
        if (nullptr != m_ErrBlob)
        {
            char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
            MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
        }
        else
        {
            MessageBoxA(nullptr, "Shader File No Exist", "Shader Compile Failed!!", MB_OK);
        }

        return E_FAIL;
    }

    DEVICE->CreateHullShader(m_HSBlob->GetBufferPointer(), m_HSBlob->GetBufferSize(), nullptr, m_HS.GetAddressOf());

    return S_OK;
}

int CGraphicsShader::CreateDomainShader(const wstring& _strRelativePath, const string& _strFuncName)
{
    wstring strContentPath = CPathMgr::GetContentPath();
    wstring strFilePath = strContentPath + _strRelativePath;

    if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "ds_5_0", m_CompileFlags, 0,
                                  m_DSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
    {
        if (nullptr != m_ErrBlob)
        {
            char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
            MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
        }
        else
        {
            MessageBoxA(nullptr, "Shader File No Exist", "Shader Compile Failed!!", MB_OK);
        }

        return E_FAIL;
    }

    DEVICE->CreateDomainShader(m_DSBlob->GetBufferPointer(), m_DSBlob->GetBufferSize(), nullptr, m_DS.GetAddressOf());

    return S_OK;
}

int CGraphicsShader::CreateGeometryShader(const wstring& _strRelativePath, const string& _strFuncName)
{
    wstring strContentPath = CPathMgr::GetContentPath();
    wstring strFilePath = strContentPath + _strRelativePath;

    if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "gs_5_0", m_CompileFlags, 0,
                                  m_GSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
    {
        if (nullptr != m_ErrBlob)
        {
            char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
            MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
        }
        else
        {
            MessageBoxA(nullptr, "Shader File No Exist", "Shader Compile Failed!!", MB_OK);
        }

        return E_FAIL;
    }

    DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(), nullptr, m_GS.GetAddressOf());

    return S_OK;
}

int CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName)
{
    wstring strContentPath = CPathMgr::GetContentPath();
    wstring strFilePath = strContentPath + _strRelativePath;

    // 픽셀 쉐이더 생성
    // 픽셀 쉐이더 컴파일
    if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(), "ps_5_0", m_CompileFlags, 0,
                                  m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
    {
        if (nullptr != m_ErrBlob)
        {
            char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
            MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
        }
        else
        {
            MessageBoxA(nullptr, "Shader File No Exist", "Shader Compile Failed!!", MB_OK);
        }

        return E_FAIL;
    }

    DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

    return S_OK;
}

int CGraphicsShader::UpdateData()
{
    CONTEXT->IASetInputLayout(m_Layout.Get());
    CONTEXT->IASetPrimitiveTopology(m_Topology);

    CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType).Get());
    CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType).Get(), 1);
    CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType).Get(), nullptr, 0xffffffff);

    CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
    CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
    CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
    CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
    CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

    return S_OK;
}

int CGraphicsShader::UpdateData_Inst()
{
    CONTEXT->IASetInputLayout(m_LayoutInst.Get());
    CONTEXT->IASetPrimitiveTopology(m_Topology);

    CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType).Get());
    CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType).Get(), 0);
    CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType).Get(), nullptr, 0xffffffff);

    CONTEXT->VSSetShader(m_VSInst.Get(), nullptr, 0);
    CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
    CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
    CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
    CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

    return S_OK;
}
