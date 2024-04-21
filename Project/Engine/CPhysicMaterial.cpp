#include "pch.h"
#include "CPhysicMaterial.h"

CPhysicMaterial::CPhysicMaterial(bool _bEngineAsset)
    : CAsset(ASSET_TYPE::PHYSIC_MATERIAL, _bEngineAsset)
    , m_DynamicFriction(0.6f)
    , m_StaticFriction(0.6f)
    , m_Bounciness(0.f)
    , m_FrictionCombine(PHYSICMATERIAL_COMBINEMODE::Average)
    , m_BounceCombine(PHYSICMATERIAL_COMBINEMODE::Average)
{
}

CPhysicMaterial::~CPhysicMaterial()
{
}

int CPhysicMaterial::Save(const wstring& _strRelativePath)
{
    if (IsEngineAsset())
        return E_FAIL;

    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    fwrite(&m_DynamicFriction, sizeof(float), 1, pFile);
    fwrite(&m_StaticFriction, sizeof(float), 1, pFile);
    fwrite(&m_Bounciness, sizeof(float), 1, pFile);
    fwrite(&m_FrictionCombine, sizeof(PHYSICMATERIAL_COMBINEMODE), 1, pFile);
    fwrite(&m_BounceCombine, sizeof(PHYSICMATERIAL_COMBINEMODE), 1, pFile);

    fclose(pFile);

    return S_OK;
}

int CPhysicMaterial::Load(const wstring& _strFilePath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    if (nullptr == pFile)
        return E_FAIL;

    fread(&m_DynamicFriction, sizeof(float), 1, pFile);
    fread(&m_StaticFriction, sizeof(float), 1, pFile);
    fread(&m_Bounciness, sizeof(float), 1, pFile);
    fread(&m_FrictionCombine, sizeof(PHYSICMATERIAL_COMBINEMODE), 1, pFile);
    fread(&m_BounceCombine, sizeof(PHYSICMATERIAL_COMBINEMODE), 1, pFile);

    fclose(pFile);

    return S_OK;
}
