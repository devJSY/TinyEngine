#include "pch.h"
#include "CMaterial.h"

#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTexture.h"
#include "COutputLog.h"

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
    for (UINT i = TEX_PARAM::TEX_0; i < TEX_PARAM::END; ++i)
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

int CMaterial::Save(const wstring& _strFilePath)
{
    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _strFilePath;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    if (nullptr == pFile)
    {
        LOG(Error, "파일 열기 실패");
        return E_FAIL;
    }

    // Entity
    SaveWString(GetName(), pFile);

    // Res
    SaveWString(GetKey(), pFile);

    // Shader
    SaveAssetRef(m_pShader.Get(), pFile);

    // Constant
    fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);

    // Texture
    for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
    {
        SaveAssetRef(m_arrTex[i].Get(), pFile);
    }

    fclose(pFile);

    return S_OK;
}

int CMaterial::Load(const wstring& _strFilePath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    if (nullptr == pFile)
    {
        LOG(Error, "파일 열기 실패");
        return E_FAIL;
    }

    // Entity
    wstring strName;
    LoadWString(strName, pFile);
    SetName(strName);

    // Res
    wstring strKey;
    LoadWString(strKey, pFile);

    // Shader
    LoadAssetRef(m_pShader, pFile);

    // Constant
    fread(&m_Const, sizeof(tMtrlConst), 1, pFile);

    // Texture
    for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
    {
        LoadAssetRef(m_arrTex[i], pFile);
    }

    fclose(pFile);

    return S_OK;
}
