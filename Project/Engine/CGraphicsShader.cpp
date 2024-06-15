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
    // 정점 구조정보(Layout) 생성
    D3D11_INPUT_ELEMENT_DESC arrElement[25] = {}; // 정점하나안의 요소 갯수만큼 생성

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

    arrElement[3].InputSlot = 0;
    arrElement[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    arrElement[3].SemanticName = "TANGENT";
    arrElement[3].SemanticIndex = 0;
    arrElement[3].InstanceDataStepRate = 0;
    arrElement[3].AlignedByteOffset = 36;
    arrElement[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;

    arrElement[4].InputSlot = 0;
    arrElement[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    arrElement[4].SemanticName = "BINORMAL";
    arrElement[4].SemanticIndex = 0;
    arrElement[4].InstanceDataStepRate = 0;
    arrElement[4].AlignedByteOffset = 48;
    arrElement[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;

    arrElement[5].InputSlot = 0;
    arrElement[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    arrElement[5].SemanticName = "NORMAL";
    arrElement[5].SemanticIndex = 0;
    arrElement[5].InstanceDataStepRate = 0;
    arrElement[5].AlignedByteOffset = 60;
    arrElement[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;

    arrElement[6].SemanticName = "BLENDWEIGHT";
    arrElement[6].SemanticIndex = 0;
    arrElement[6].AlignedByteOffset = 72;
    arrElement[6].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[6].InputSlot = 0;
    arrElement[6].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    arrElement[6].InstanceDataStepRate = 0;

    arrElement[7].SemanticName = "BLENDINDICES";
    arrElement[7].SemanticIndex = 0;
    arrElement[7].AlignedByteOffset = 88;
    arrElement[7].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[7].InputSlot = 0;
    arrElement[7].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    arrElement[7].InstanceDataStepRate = 0;

    // 두번째 슬롯
    arrElement[8].SemanticName = "WORLD";
    arrElement[8].SemanticIndex = 0;
    arrElement[8].AlignedByteOffset = 0;
    arrElement[8].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[8].InputSlot = 1;
    arrElement[8].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[8].InstanceDataStepRate = 1;

    arrElement[9].SemanticName = "WORLD";
    arrElement[9].SemanticIndex = 1;
    arrElement[9].AlignedByteOffset = 16;
    arrElement[9].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[9].InputSlot = 1;
    arrElement[9].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[9].InstanceDataStepRate = 1;

    arrElement[10].SemanticName = "WORLD";
    arrElement[10].SemanticIndex = 2;
    arrElement[10].AlignedByteOffset = 32;
    arrElement[10].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[10].InputSlot = 1;
    arrElement[10].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[10].InstanceDataStepRate = 1;

    arrElement[11].SemanticName = "WORLD";
    arrElement[11].SemanticIndex = 3;
    arrElement[11].AlignedByteOffset = 48;
    arrElement[11].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[11].InputSlot = 1;
    arrElement[11].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[11].InstanceDataStepRate = 1;

    arrElement[12].SemanticName = "WORLDINVTRANSPOSE";
    arrElement[12].SemanticIndex = 0;
    arrElement[12].AlignedByteOffset = 64;
    arrElement[12].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[12].InputSlot = 1;
    arrElement[12].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[12].InstanceDataStepRate = 1;

    arrElement[13].SemanticName = "WORLDINVTRANSPOSE";
    arrElement[13].SemanticIndex = 1;
    arrElement[13].AlignedByteOffset = 80;
    arrElement[13].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[13].InputSlot = 1;
    arrElement[13].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[13].InstanceDataStepRate = 1;

    arrElement[14].SemanticName = "WORLDINVTRANSPOSE";
    arrElement[14].SemanticIndex = 2;
    arrElement[14].AlignedByteOffset = 96;
    arrElement[14].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[14].InputSlot = 1;
    arrElement[14].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[14].InstanceDataStepRate = 1;

    arrElement[15].SemanticName = "WORLDINVTRANSPOSE";
    arrElement[15].SemanticIndex = 3;
    arrElement[15].AlignedByteOffset = 112;
    arrElement[15].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[15].InputSlot = 1;
    arrElement[15].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[15].InstanceDataStepRate = 1;

    arrElement[16].SemanticName = "WV";
    arrElement[16].SemanticIndex = 0;
    arrElement[16].AlignedByteOffset = 128;
    arrElement[16].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[16].InputSlot = 1;
    arrElement[16].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[16].InstanceDataStepRate = 1;

    arrElement[17].SemanticName = "WV";
    arrElement[17].SemanticIndex = 1;
    arrElement[17].AlignedByteOffset = 144;
    arrElement[17].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[17].InputSlot = 1;
    arrElement[17].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[17].InstanceDataStepRate = 1;

    arrElement[18].SemanticName = "WV";
    arrElement[18].SemanticIndex = 2;
    arrElement[18].AlignedByteOffset = 160;
    arrElement[18].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[18].InputSlot = 1;
    arrElement[18].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[18].InstanceDataStepRate = 1;

    arrElement[19].SemanticName = "WV";
    arrElement[19].SemanticIndex = 3;
    arrElement[19].AlignedByteOffset = 176;
    arrElement[19].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[19].InputSlot = 1;
    arrElement[19].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[19].InstanceDataStepRate = 1;

    arrElement[20].SemanticName = "WVP";
    arrElement[20].SemanticIndex = 0;
    arrElement[20].AlignedByteOffset = 192;
    arrElement[20].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[20].InputSlot = 1;
    arrElement[20].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[20].InstanceDataStepRate = 1;

    arrElement[21].SemanticName = "WVP";
    arrElement[21].SemanticIndex = 1;
    arrElement[21].AlignedByteOffset = 208;
    arrElement[21].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[21].InputSlot = 1;
    arrElement[21].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[21].InstanceDataStepRate = 1;

    arrElement[22].SemanticName = "WVP";
    arrElement[22].SemanticIndex = 2;
    arrElement[22].AlignedByteOffset = 224;
    arrElement[22].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[22].InputSlot = 1;
    arrElement[22].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[22].InstanceDataStepRate = 1;

    arrElement[23].SemanticName = "WVP";
    arrElement[23].SemanticIndex = 3;
    arrElement[23].AlignedByteOffset = 240;
    arrElement[23].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    arrElement[23].InputSlot = 1;
    arrElement[23].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[23].InstanceDataStepRate = 1;

    arrElement[24].SemanticName = "ROWINDEX";
    arrElement[24].SemanticIndex = 0;
    arrElement[24].AlignedByteOffset = 256;
    arrElement[24].Format = DXGI_FORMAT_R32_UINT;
    arrElement[24].InputSlot = 1;
    arrElement[24].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    arrElement[24].InstanceDataStepRate = 1;

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
    DEVICE->CreateInputLayout(arrElement, 25, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), m_Layout.GetAddressOf());

    // Vertex Inst Shader 컴파일 하기
    HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, (_strFuncName + "_Inst").c_str(), "vs_5_0",
                                    m_CompileFlags, 0, m_VSInstBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

    if (SUCCEEDED(hr))
    {
        // 컴파일 된 코드로 Vertex Instancing Shader 객체 만들기
        DEVICE->CreateVertexShader(m_VSInstBlob->GetBufferPointer(), m_VSInstBlob->GetBufferSize(), nullptr, m_VSInst.GetAddressOf());

        DEVICE->CreateInputLayout(arrElement, 25, m_VSInstBlob->GetBufferPointer(), m_VSInstBlob->GetBufferSize(), m_LayoutInst.GetAddressOf());
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
