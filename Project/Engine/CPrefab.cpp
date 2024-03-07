#include "pch.h"
#include "CPrefab.h"

#include "CLevelSaveLoad.h"
#include "CGameObject.h"

SAVE_TYPE CPrefab::GAMEOBJECT_SAVE = &CLevelSaveLoad::SaveGameObject;
LOAD_TYPE CPrefab::GAMEOBJECT_LOAD = &CLevelSaveLoad::LoadGameObject;

CPrefab::CPrefab(bool _bEngineAsset)
    : CAsset(ASSET_TYPE::PREFAB, _bEngineAsset)
    , m_ProtoObj(nullptr)
{
}

CPrefab::CPrefab(CGameObject* _Proto, bool _bEngineAsset)
    : CAsset(ASSET_TYPE::PREFAB, _bEngineAsset)
    , m_ProtoObj(_Proto)
{
}

CPrefab::~CPrefab()
{
    if (nullptr != m_ProtoObj)
        delete m_ProtoObj;
}

void CPrefab::SetGameObject(CGameObject* _Object)
{
    if (nullptr != m_ProtoObj)
        delete m_ProtoObj;

    m_ProtoObj = _Object;
}

CGameObject* CPrefab::Instantiate()
{
    return m_ProtoObj->Clone();
}

int CPrefab::Save(const wstring& _strRelativePath)
{
    assert(GAMEOBJECT_SAVE);

    wstring strContentPath = CPathMgr::GetContentPath();
    strContentPath += _strRelativePath;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strContentPath.c_str(), L"wb");

    if (nullptr == pFile)
    {
        MessageBox(nullptr, L"Prefab 저장 실패", L"Prefab 저장 실패", MB_OK);
        return E_FAIL;
    }

    GAMEOBJECT_SAVE(m_ProtoObj, pFile);

    fclose(pFile);
    return S_OK;
}

int CPrefab::Load(const wstring& _strFilePath)
{
    assert(GAMEOBJECT_LOAD);

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    if (nullptr == pFile)
    {
        MessageBox(nullptr, L"Prefab 로딩 실패", L"Prefab 로딩 실패", MB_OK);
        return E_FAIL;
    }

    m_ProtoObj = GAMEOBJECT_LOAD(pFile);

    fclose(pFile);
    return S_OK;
}