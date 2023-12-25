#pragma once
#include "singleton.h"

#include "CPathMgr.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CGraphicsShader.h"

class CMesh;
class CGraphicsShader;

class CAssetMgr : public CSingleton<CAssetMgr>
{
    SINGLE(CAssetMgr);

private:
    map<wstring, CAsset*> m_mapAsset[(UINT)ASSET_TYPE::END];
    map<wstring, vector<CMesh*>> m_mapModel;

public:
    void init();

private:
    // Geometry
    tMeshData MakeCircle(const float radius, const int numSlices);
    tMeshData MakeSquare(const float scale = 1.0f, const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeSquareGrid(const int numSlices, const int numStacks, const float scale = 1.0f,
                             const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeBox(const float scale = 1.0f);
    tMeshData MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices);
    tMeshData MakeSphere(const float radius, const int numSlices, const int numStacks,
                         const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeTetrahedron(); // 사면체
    tMeshData MakeIcosahedron(); // 이십면체
    tMeshData SubdivideToSphere(const float radius, tMeshData meshData);

    // 모델 로딩
private:
    vector<tMeshData> ReadFromFile(std::string basePath, std::string filename, bool revertNormals = false);
    void MeshTextureLoad(CMesh* pMesh, const tMeshData& meshData);

public:
    void AddModel(const wstring& _strKey, vector<CMesh*> _model);
    vector<CMesh*> FindModel(const wstring& _strKey);

public:
    template <typename T>
    void AddAsset(const wstring& _strKey, T* _Asset);

    template <typename T>
    T* FindAsset(const wstring& _strKey);

    template <typename T>
    T* Load(const wstring& _strKey, const wstring& _strRelativePath);
};

template <typename T>
ASSET_TYPE GetAssetType()
{
    const type_info& info = typeid(T);

    ASSET_TYPE Type = ASSET_TYPE::END;

    if (&info == &typeid(CMesh))
        Type = ASSET_TYPE::MESH;
    else if (&info == &typeid(CTexture))
        Type = ASSET_TYPE::TEXTURE;
    else if (&info == &typeid(CMaterial))
        Type = ASSET_TYPE::MATERIAL;
    else if (&info == &typeid(CGraphicsShader))
        Type = ASSET_TYPE::GRAPHICS_SHADER;

    return Type;
}

template <typename T>
inline void CAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, CAsset*>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
    assert(iter == m_mapAsset[(UINT)Type].end());

    m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
}

template <typename T>
inline T* CAssetMgr::FindAsset(const wstring& _strKey)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, CAsset*>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

    if (iter == m_mapAsset[(UINT)Type].end())
    {
        return nullptr;
    }

    return (T*)iter->second;
}

template <typename T>
inline T* CAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    CAsset* pAsset = FindAsset<T>(_strKey);

    // 로딩할 때 사용할 키로 이미 다른 에셋이 있다면
    if (nullptr != pAsset)
    {
        return (T*)pAsset;
    }

    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    pAsset = new T;
    if (FAILED(pAsset->Load(strFilePath)))
    {
        MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 실패", MB_OK);
        delete pAsset;
        return nullptr;
    }

    pAsset->SetKey(_strKey);
    pAsset->SetRelativePath(_strRelativePath);
    AddAsset<T>(_strKey, (T*)pAsset);

    return (T*)pAsset;
}