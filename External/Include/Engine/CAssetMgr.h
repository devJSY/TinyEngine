#pragma once
#include "singleton.h"

#include "CPathMgr.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CGraphicsShader.h"

class CGameObject;
class CMesh;
class CGraphicsShader;

class CAssetMgr : public CSingleton<CAssetMgr>
{
    SINGLE(CAssetMgr);

private:
    map<wstring, Ptr<CAsset>> m_mapAsset[(UINT)ASSET_TYPE::END];

public:
    void init();

private:
    // 에셋 로딩
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultTexture();
    void CreateDefaultMaterial();

private:
    // Geometry
    tMeshData MakeCircle(const float radius, const int numSlices);
    tMeshData MakeRect(const float scale = 1.0f, const Vec2 texScale = Vec2(1.0f));
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
public:
    CGameObject* LoadModel(const std::string& _relativepath, const std::string& _filename, const std::wstring& _name,
                           bool revertNormals = false);

private:
    vector<tMeshData> ReadFromFile(std::string basePath, std::string filename, bool revertNormals);
    Ptr<CMaterial> LoadModelMaterial(Ptr<CMesh> _Mesh, const tMeshData& _MeshData);

public:
    Ptr<CTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat,
                                UINT _BindFlag, D3D11_USAGE _Usage);
    Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D);

public:
    template <typename T>
    void AddAsset(const wstring& _strKey, Ptr<T> _Asset);

    template <typename T>
    Ptr<T> FindAsset(const wstring& _strKey);

    template <typename T>
    Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath);

private:
    void DeleteAsset(ASSET_TYPE _type, const wstring& _strKey);

    friend class CTaskMgr;
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
inline void CAssetMgr::AddAsset(const wstring& _strKey, Ptr<T> _Asset)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
    assert(iter == m_mapAsset[(UINT)Type].end());

    m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset.Get()));
}

template <typename T>
Ptr<T> CAssetMgr::FindAsset(const wstring& _strKey)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);

    if (iter == m_mapAsset[(UINT)Type].end())
    {
        return nullptr;
    }

    // CAsset 타입을 요청한 타입으로 캐스팅해서 반환
    return (T*)iter->second.Get();
}

template <typename T>
Ptr<T> CAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    Ptr<T> pAsset = FindAsset<T>(_strKey);

    // 로딩할 때 사용할 키로 이미 다른 에셋이 있다면
    if (nullptr != pAsset)
    {
        return (T*)pAsset.Get();
    }

    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    pAsset = new T;
    if (FAILED(pAsset->Load(strFilePath)))
    {
        MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 실패", MB_OK);
        pAsset = nullptr;
        return nullptr;
    }

    pAsset->SetKey(_strKey);
    pAsset->SetRelativePath(_strRelativePath);
    AddAsset<T>(_strKey, (T*)pAsset.Get());

    // CAsset 타입을 요청한 타입으로 캐스팅해서 반환
    return (T*)pAsset.Get();
}