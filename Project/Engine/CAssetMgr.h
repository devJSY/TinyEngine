#pragma once
#include "singleton.h"

#include "CPathMgr.h"

#include "CMesh.h"
#include "CPrefab.h"
#include "CTexture.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CMaterial.h"
#include "CSound.h"
#include "CPhysics2DMaterial.h"
#include "CPhysicMaterial.h"
#include "CMeshData.h"

class CAssetMgr : public CSingleton<CAssetMgr>
{
    SINGLE(CAssetMgr);

private:
    map<wstring, Ptr<CAsset>> m_mapAsset[(UINT)ASSET_TYPE::END];

    vector<D3D11_INPUT_ELEMENT_DESC> m_vecLayoutInfo;
    UINT m_iLayoutOffset_0;
    UINT m_iLayoutOffset_1;

public:
    const map<wstring, Ptr<CAsset>>& GetMapAsset(ASSET_TYPE _type) const { return m_mapAsset[(UINT)_type]; }
    const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutInfo() const { return m_vecLayoutInfo; }

public:
    void init();
    void initSound();

private:
    void SaveAssetsToFile();
    void LoadAssetsFromFile(std::filesystem::path _EntryPath);

public:
    void ReloadContent();

private:
    // ���� ����
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultComputeShader();
    void CreateDefaultMaterial();
    void CreateDefaultPhysics2DMaterial();
    void CreateDefaultPhysicMaterial();

    void AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName, UINT _iSlotNum, UINT _iSemanticIdx);

public:
    Ptr<CMeshData> LoadFBX(const wstring& _strPath);
    void LoadAnimationFBX(Ptr<CMesh> _pOriginMesh, const wstring& _strPath);
   
public:
    // Geometry Function
    tMeshData MakePoint();
    tMeshData MakeCrosshair();
    tMeshData MakeCircle(const float radius, const int numSlices);
    tMeshData MakeRect(const float scale = 1.f, const Vec2 texScale = Vec2(1.f));
    tMeshData MakeDebugCircle(const float radius, const int numSlices);
    tMeshData MakeDebugRect(const float scale = 1.f, const Vec2 texScale = Vec2(1.f));
    tMeshData MakeSquareGrid(const int numSlices, const int numStacks, const float scale = 1.f, const Vec2 texScale = Vec2(1.f));
    tMeshData MakeBox(const float scale = 1.f);
    tMeshData MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices);
    tMeshData MakeSphere(const float radius, const int numSlices, const int numStacks, const Vec2 texScale = Vec2(1.f));
    tMeshData MakeCapsule(const float radius, const float halfHeight, const int numSlices);
    tMeshData MakeTetrahedron(); // ���ü
    tMeshData MakeIcosahedron(); // �̽ʸ�ü
    tMeshData SubdivideToSphere(const float radius, tMeshData meshData);
    tMeshData MakeCone(const float radius = 0.5f, const float height = 1.f);

    tMeshData MakeWireCircle(const float radius, const int numPoints);
    tMeshData MakeWireBox(const float scale = 1.0f);
    tMeshData MakeWireSphere(const float radius, const int numPoints);
    tMeshData MakeWireCapsule2D(const float radius, const float halfHeight, const int numPoints);
    tMeshData MakeWireCapsule(const float radius, const float halfHeight, const int numPoints);

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

    template <typename T>
    Ptr<T> Load(const wstring& _strRelativePath);

    void ReplacePrefab(Ptr<CPrefab> _Prefab, const wstring& _strKey);

private:
    void DeleteAsset(ASSET_TYPE _type, const wstring& _strKey);

    friend class CTaskMgr;
    friend class CDevice;
    friend class CRenderMgr;
    friend class COutliner;
    friend class CModelEditor;
};

template <typename T>
ASSET_TYPE GetAssetType()
{
    ASSET_TYPE Type = ASSET_TYPE::END;

    if constexpr (std::is_same_v<CMesh, T>)
        Type = ASSET_TYPE::MESH;
    if constexpr (std::is_same_v<CMeshData, T>)
        Type = ASSET_TYPE::MESHDATA;
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
    if constexpr (std::is_same_v<CSound, T>)
        Type = ASSET_TYPE::SOUND;
    if constexpr (std::is_same_v<CPhysics2DMaterial, T>)
        Type = ASSET_TYPE::PHYSICS2D_MATERIAL;
    if constexpr (std::is_same_v<CPhysicMaterial, T>)
        Type = ASSET_TYPE::PHYSIC_MATERIAL;

    return Type;
}

template <typename T>
inline void CAssetMgr::AddAsset(const wstring& _strKey, Ptr<T> _Asset)
{
    ASSET_TYPE Type = GetAssetType<T>();

    if (nullptr != FindAsset<T>(_strKey))
    {
        MessageBox(nullptr, L"������ Ű�� ������ �̹� �����մϴ�!", L"���� �߰� ����", MB_OK);
        return;
    }

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

    // CAsset Ÿ���� ��û�� Ÿ������ ĳ�����ؼ� ��ȯ
    return (T*)iter->second.Get();
}

template <typename T>
Ptr<T> CAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    Ptr<CAsset> pAsset = FindAsset<T>(_strKey).Get();

    // �ε��� �� ����� Ű�� �̹� �ٸ� ������ �ִٸ�
    if (nullptr != pAsset)
    {
        return (T*)pAsset.Get();
    }

    wstring strFilePath = CPathMgr::GetContentPath();
    strFilePath += _strRelativePath;

    pAsset = new T;
    if (FAILED(pAsset->Load(strFilePath)))
    {
        MessageBox(nullptr, L"���� �ε� ����", L"���� �ε� ����", MB_OK);
        pAsset = nullptr;
        return nullptr;
    }

    AddAsset<T>(_strKey, (T*)pAsset.Get());

    // CAsset Ÿ���� ��û�� Ÿ������ ĳ�����ؼ� ��ȯ
    return (T*)pAsset.Get();
}

template <typename T>
Ptr<T> CAssetMgr::Load(const wstring& _strRelativePath)
{
    return Load<T>(_strRelativePath, _strRelativePath);
}
