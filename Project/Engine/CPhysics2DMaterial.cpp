#include "pch.h"
#include "CPhysics2DMaterial.h"

CPhysics2DMaterial::CPhysics2DMaterial(bool _bEngineAsset)
    : CAsset(ASSET_TYPE::PHYSICS2D_MATERIAL, _bEngineAsset)
    , m_Friction(0.4f)
    , m_Bounciness(0.f)
{
}

CPhysics2DMaterial::~CPhysics2DMaterial()
{
}

int CPhysics2DMaterial::Save(const wstring& _strRelativePath)
{
    if (IsEngineAsset())
        return E_FAIL;

    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    fwrite(&m_Friction, sizeof(float), 1, pFile);
    fwrite(&m_Bounciness, sizeof(float), 1, pFile);

    fclose(pFile);

    return S_OK;
}

int CPhysics2DMaterial::Load(const wstring& _strFilePath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    if (nullptr == pFile)
        return E_FAIL;

    fread(&m_Friction, sizeof(float), 1, pFile);
    fread(&m_Bounciness, sizeof(float), 1, pFile);

    fclose(pFile);

    return S_OK;
}
