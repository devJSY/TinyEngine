#pragma once
#include "singleton.h"

#include "CPathMgr.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"

class CGameObject;
class CAsset;
class CMesh;
class CGraphicsShader;
class CComputeShader;

class CAssetMgr : public CSingleton<CAssetMgr>
{
    SINGLE(CAssetMgr);

private:
    map<wstring, Ptr<CAsset>> m_mapAsset[(UINT)ASSET_TYPE::END];
    wstring m_AssetExtension;

public:
    const map<wstring, Ptr<CAsset>>& GetMapAsset(ASSET_TYPE _type) const { return m_mapAsset[(UINT)_type]; }

public:
    void init();

private:
    // 에셋 생성
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultComputeShader();
    void CreateDefaultTexture();
    void CreateDefaultMaterial();

    void LoadFromAssetFile();

public:
    // Geometry
    tMeshData MakeCrosshair();
    tMeshData MakePoint();
    tMeshData MakeCircle(const float radius, const int numSlices);
    tMeshData MakeRect(const float scale = 1.0f, const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeDebugCircle(const float radius, const int numSlices);
    tMeshData MakeDebugRect(const float scale = 1.0f, const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeSquareGrid(const int numSlices, const int numStacks, const float scale = 1.0f,
                             const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeBox(const float scale = 1.0f);
    tMeshData MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices);
    tMeshData MakeSphere(const float radius, const int numSlices, const int numStacks,
                         const Vec2 texScale = Vec2(1.0f));
    tMeshData MakeTetrahedron(); // 사면체
    tMeshData MakeIcosahedron(); // 이십면체
    tMeshData SubdivideToSphere(const float radius, tMeshData meshData);

    tMeshData MakeWireBox(const Vector3 center, const Vector3 extents);
    tMeshData MakeWireSphere(const Vector3 center, const float radius);

    // 모델 로딩
public:
    CGameObject* LoadModel(const wstring& _name, string _basePath, string _filename, bool _revertNormals = false,
                           tMeshData _TexturesName = {});
    CGameObject* LoadModel(const wstring& _name, vector<tMeshData> meshes);

private:
    vector<tMeshData> ReadFromFile(string _basePath, string _filename, bool _revertNormals);
    Ptr<CMaterial> LoadModelMaterial(Ptr<CMesh> _Mesh, const tMeshData& _MeshData);

public:
    Ptr<CTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat,
                                UINT _BindFlag, D3D11_USAGE _Usage,
                                const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc = nullptr,
                                const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc = nullptr,
                                const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc = nullptr,
                                const D3D11_UNORDERED_ACCESS_VIEW_DESC* _uavDesc = nullptr);
    Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D,
                                const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc = nullptr,
                                const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc = nullptr,
                                const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc = nullptr,
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
    const type_info& info = typeid(T);

    ASSET_TYPE Type = ASSET_TYPE::END;

    if (&info == &typeid(CMesh))
        Type = ASSET_TYPE::MESH;
    else if (&info == &typeid(CTexture))
        Type = ASSET_TYPE::TEXTURE;
    else if (&info == &typeid(CGraphicsShader))
        Type = ASSET_TYPE::GRAPHICS_SHADER;
    else if (&info == &typeid(CComputeShader))
        Type = ASSET_TYPE::COMPUTE_SHADER;
    else if (&info == &typeid(CMaterial))
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
    _Asset->SetKey(_strKey);
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

    pAsset->SetKey(_strKey);
    pAsset->SetRelativePath(_strRelativePath);
    AddAsset<T>(_strKey, (T*)pAsset.Get());

    // CAsset 타입을 요청한 타입으로 캐스팅해서 반환
    return (T*)pAsset.Get();
}