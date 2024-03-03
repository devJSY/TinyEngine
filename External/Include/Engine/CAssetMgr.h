#pragma once
#include "singleton.h"

#include "CPathMgr.h"

#include "CMesh.h"
#include "CPrefab.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CMaterial.h"

class CAssetMgr : public CSingleton<CAssetMgr>
{
    SINGLE(CAssetMgr);

private:
    map<wstring, Ptr<CAsset>> m_mapAsset[(UINT)ASSET_TYPE::END];

public:
    const map<wstring, Ptr<CAsset>>& GetMapAsset(ASSET_TYPE _type) const { return m_mapAsset[(UINT)_type]; }

public:
    void init();
    void ReloadContent();

private:
    void LoadAssetsFromFile(std::filesystem::path _EntryPath);

private:
    // 에셋 생성
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultComputeShader();
    void CreateDefaultMaterial();

public:
    // Geometry
    tMeshData MakeCrosshair();
    tMeshData MakePoint();
    tMeshData MakeCircle(const float radius, const int numSlices);
    tMeshData MakeRect(const float scale = 1.0f, const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeDebugCircle(const float radius, const int numSlices);
    tMeshData MakeDebugRect(const float scale = 1.0f, const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeSquareGrid(const int numSlices, const int numStacks, const float scale = 1.0f, const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeBox(const float scale = 1.0f);
    tMeshData MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices);
    tMeshData MakeSphere(const float radius, const int numSlices, const int numStacks, const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeTetrahedron(); // 사면체
    tMeshData MakeIcosahedron(); // 이십면체
    tMeshData SubdivideToSphere(const float radius, tMeshData meshData);

    tMeshData MakeWireBox(const Vector3 center, const Vector3 extents);
    tMeshData MakeWireSphere(const Vector3 center, const float radius);

    // 모델 로딩
public:
    CGameObject* LoadModel(const wstring& _name, string _basePath, string _filename, bool _revertNormals = false, tMeshData _TexturesName = {});
    CGameObject* LoadModel(const wstring& _name, vector<tMeshData> meshes);

private:
    vector<tMeshData> ReadFromFile(string _basePath, string _filename, bool _revertNormals);
    void SetModelMaterial(const Ptr<CMaterial>& _Mtrl, const tMeshData& _MeshData);

public:
    Ptr<CTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage,
                                const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc = nullptr, const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc = nullptr,
                                const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc = nullptr,
                                const D3D11_UNORDERED_ACCESS_VIEW_DESC* _uavDesc = nullptr);
    Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D, const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc = nullptr,
                                const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc = nullptr, const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc = nullptr,
                                const D3D11_UNORDERED_ACCESS_VIEW_DESC* _uavDesc = nullptr);

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
    friend class CDevice;
    friend class CRenderMgr;
};

template <typename T>
ASSET_TYPE GetAssetType()
{
    ASSET_TYPE Type = ASSET_TYPE::END;

    if constexpr (std::is_same_v<CMesh, T>)
        Type = ASSET_TYPE::MESH;
    if constexpr (std::is_same_v<CPrefab, T>)
        Type = ASSET_TYPE::PREFAB;
    if constexpr (std::is_same_v<CTexture, T>)
        Type = ASSET_TYPE::TEXTURE;
    if constexpr (std::is_same_v<CGraphicsShader, T>)
        Type = ASSET_TYPE::GRAPHICS_SHADER;
    if constexpr (std::is_same_v<CComputeShader, T>)
        Type = ASSET_TYPE::COMPUTE_SHADER;
    if constexpr (std::is_same_v<CMaterial, T>)
        Type = ASSET_TYPE::MATERIAL;

    return Type;
}

template <typename T>
inline void CAssetMgr::AddAsset(const wstring& _strKey, Ptr<T> _Asset)
{
    ASSET_TYPE Type = GetAssetType<T>();

    map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_strKey);
    assert(iter == m_mapAsset[(UINT)Type].end());

    m_mapAsset[(UINT)Type].insert(make_pair(_strKey, _Asset.Get()));
    _Asset->SetName(_strKey);
    _Asset->SetKey(_strKey);
    _Asset->SetRelativePath(_strKey);
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
    Ptr<CAsset> pAsset = FindAsset<T>(_strKey).Get();

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

    AddAsset<T>(_strKey, (T*)pAsset.Get());

    // CAsset 타입을 요청한 타입으로 캐스팅해서 반환
    return (T*)pAsset.Get();
}