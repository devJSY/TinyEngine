#include "pch.h"
#include "CComputeShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CPathMgr.h"

CComputeShader::CComputeShader(UINT _ThreadX, UINT _ThreadY, UINT _ThreadZ)
    : CShader(ASSET_TYPE::COMPUTE_SHADER)
    , m_GroupX(1)
    , m_GroupY(1)
    , m_GroupZ(1)
    , m_ThreadX(_ThreadX)
    , m_ThreadY(_ThreadY)
    , m_ThreadZ(_ThreadZ)
{
}

CComputeShader::~CComputeShader()
{
}

int CComputeShader::Create(const wstring& _strRelativePath, const string& _strFuncName)
{
    wstring strContentPath = CPathMgr::GetContentPath();
    wstring strFilePath = strContentPath + _strRelativePath;

    // 픽셀 쉐이더 생성
    // 픽셀 쉐이더 컴파일
    if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _strFuncName.c_str(),
                                  "cs_5_0", D3DCOMPILE_DEBUG, 0, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
    {
        if (nullptr != m_ErrBlob)
        {
            char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
            MessageBoxA(nullptr, pErrMsg, "Shader Compile Failed!!", MB_OK);
        }

        return E_FAIL;
    }

    DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize(), nullptr, m_CS.GetAddressOf());

    return S_OK;
}

void CComputeShader::Execute()
{
    UpdateData();

    // 상수 데이터 바인딩
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
    pCB->SetData(&m_Const);
    pCB->UpdateData_CS();

    // 컴퓨트 쉐이더 실행
    CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

    Clear();
}
