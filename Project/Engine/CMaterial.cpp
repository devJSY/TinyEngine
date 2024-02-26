#include "pch.h"
#include "CMaterial.h"

#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTexture.h"

CMaterial::CMaterial(bool _bEngineAsset)
    : CAsset(ASSET_TYPE::MATERIAL, _bEngineAsset)
    , m_Const{}
    , m_arrTex{}
    , m_pShader(nullptr)
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
    if (nullptr == m_pShader.Get())
        return;

    // 사용할 쉐이더 바인딩
    m_pShader->UpdateData();

    // Texture Update(Register Binding)
    for (UINT i = TEX_PARAM::TEX_0; i < TEX_PARAM::TEX_END; ++i)
    {
        if (nullptr != m_arrTex[i].Get())
        {
            m_arrTex[i]->UpdateData(i);
            m_Const.bTex[i] = 1;
        }
        else
        {
            CTexture::Clear(i);
            m_Const.bTex[i] = 0;
        }
    }

    // 상수 데이터 업데이트
    static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
    pCB->SetData(&m_Const);
    pCB->UpdateData();
}

void* CMaterial::GetScalarParam(SCALAR_PARAM _ParamType)
{
    switch (_ParamType)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3: {
        int idx = _ParamType - INT_0;
        return m_Const.arrInt + idx;
    }
    break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3: {
        int idx = _ParamType - FLOAT_0;
        return m_Const.arrFloat + idx;
    }
    break;
    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3: {
        int idx = _ParamType - VEC2_0;
        return m_Const.arrVec2 + idx;
    }
    break;
    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3: {
        int idx = _ParamType - VEC4_0;
        return m_Const.arrVec4 + idx;
    }
    break;
    case MAT_0:
    case MAT_1:
    case MAT_2:
    case MAT_3: {
        int idx = _ParamType - MAT_0;
        return m_Const.arrMat + idx;
    }
    break;
    }

    return nullptr;
}

int CMaterial::Save(const wstring& _strRelativePath)
{
    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    // 재질 상수값 저장
    fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);

    // 재질이 참조하는 텍스쳐 정보를 저장
    for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
    {
        SaveAssetRef<CTexture>(m_arrTex[i], pFile);
    }

    // 재질이 참조하는 쉐이더 정보를 저장
    SaveAssetRef<CGraphicsShader>(m_pShader, pFile);

    return S_OK;
}

int CMaterial::Load(const wstring& _strFilePath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    if (nullptr == pFile)
        return E_FAIL;

    // 재질 상수값 저장
    fread(&m_Const, sizeof(tMtrlConst), 1, pFile);

    // 재질이 참조하는 텍스쳐 정보를 로드
    for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
    {
        LoadAssetRef<CTexture>(m_arrTex[i], pFile);
    }

    // 재질이 참조하는 쉐이더 정보를 저장
    LoadAssetRef<CGraphicsShader>(m_pShader, pFile);

    return S_OK;
}
