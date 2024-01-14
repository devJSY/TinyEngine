#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CModleLoader.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h"

CAssetMgr::CAssetMgr()
{
}

CAssetMgr::~CAssetMgr()
{
    // 모든 에셋 파일로 저장
    for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
    {
        std::wstring basePath = L"";

        switch ((ASSET_TYPE)i)
        {
        case ASSET_TYPE::MESH:
            break;
        case ASSET_TYPE::MESHDATA:
            break;
        case ASSET_TYPE::TEXTURE:
            basePath = L"Textures\\";
            break;
        case ASSET_TYPE::COMPUTE_SHADER:
            break;
        case ASSET_TYPE::GRAPHICS_SHADER:
            break;
        case ASSET_TYPE::MATERIAL:
            basePath = L"Materials\\";
            break;
        case ASSET_TYPE::SOUND:
            break;
        }

        if (basePath.empty())
            continue;

        for (const auto& iter : m_mapAsset[i])
        {
            std::wstring filePath = basePath;
            filePath += iter.first;
            filePath += m_AssetExtension;
            iter.second->Save(filePath);
        }
    }
}

void CAssetMgr::init()
{
    m_AssetExtension = L".tasset";

    CreateDefaultMesh();
    CreateDefaultGraphicsShader();
    CreateDefaultTexture();
    // CreateDefaultMaterial();

    LoadFromAssetFile();
}

void CAssetMgr::LoadFromAssetFile()
{
    for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
    {
        std::filesystem::path basePath = CPathMgr::GetContentPath();

        switch ((ASSET_TYPE)i)
        {
        case ASSET_TYPE::MESH:
            break;
        case ASSET_TYPE::MESHDATA:
            break;
        case ASSET_TYPE::COMPUTE_SHADER:
            break;
        case ASSET_TYPE::GRAPHICS_SHADER:
            break;
        case ASSET_TYPE::TEXTURE:
            basePath += L"Textures\\";
            break;
        case ASSET_TYPE::MATERIAL:
            basePath += L"Materials\\";
            break;
        case ASSET_TYPE::SOUND:
            break;
        }

        for (auto& directoryEntry : std::filesystem::directory_iterator(basePath))
        {
            const auto& path = directoryEntry.path();
            if (m_AssetExtension != path.extension())
                continue;

            switch ((ASSET_TYPE)i)
            {
            case ASSET_TYPE::MESH:
                break;
            case ASSET_TYPE::MESHDATA:
                break;
            case ASSET_TYPE::TEXTURE:
                break;
            case ASSET_TYPE::MATERIAL:
                Load<CMaterial>(path.stem(), L"Materials\\" + std::wstring(path.filename()));
                break;
            case ASSET_TYPE::SOUND:
                break;
            case ASSET_TYPE::COMPUTE_SHADER:
                break;
            case ASSET_TYPE::GRAPHICS_SHADER:
                break;
            }
        }
    }
}

vector<tMeshData> CAssetMgr::ReadFromFile(std::string basePath, std::string filename, bool revertNormals)
{
    CModelLoader modelLoader;
    modelLoader.Load(basePath, filename, revertNormals);
    vector<tMeshData>& meshes = modelLoader.meshes;

    // Normalize vertices
    Vector3 vmin(1000, 1000, 1000);
    Vector3 vmax(-1000, -1000, -1000);
    for (auto& mesh : meshes)
    {
        for (auto& v : mesh.vertices)
        {
            vmin.x = XMMin(vmin.x, v.vPos.x);
            vmin.y = XMMin(vmin.y, v.vPos.y);
            vmin.z = XMMin(vmin.z, v.vPos.z);
            vmax.x = XMMax(vmax.x, v.vPos.x);
            vmax.y = XMMax(vmax.y, v.vPos.y);
            vmax.z = XMMax(vmax.z, v.vPos.z);
        }
    }

    float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
    float dl = XMMax(XMMax(dx, dy), dz);
    float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f, cz = (vmax.z + vmin.z) * 0.5f;

    for (auto& mesh : meshes)
    {
        for (auto& v : mesh.vertices)
        {
            v.vPos.x = (v.vPos.x - cx) / dl;
            v.vPos.y = (v.vPos.y - cy) / dl;
            v.vPos.z = (v.vPos.z - cz) / dl;
        }
    }

    return meshes;
}

Ptr<CMaterial> CAssetMgr::LoadModelMaterial(Ptr<CMesh> _Mesh, const tMeshData& _MeshData)
{
    CMaterial* pMtrl = new CMaterial;
    pMtrl->SetMaterialCoefficient(Vec4(), Vec4(1.f, 1.f, 1.f, 1.f), Vec4(1.f, 1.f, 1.f, 1.f), Vec4());
    pMtrl->SetShader(FindAsset<CGraphicsShader>(L"BlinnPhongShader"));

    // 텍스쳐 로딩
    std::wstring path = stringToWstring(_MeshData.RelativeTextureFilePath);

    if (!_MeshData.AmbientTextureFilename.empty())
    {
        std::wcout << _Mesh->GetName() << " : ";
        std::cout << _MeshData.AmbientTextureFilename << std::endl;

        std::wstring name = stringToWstring(_MeshData.AmbientTextureFilename);

        pMtrl->SetTexParam(TEX_0, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.AoTextureFilename.empty())
    {
        std::wcout << _Mesh->GetName() << " : ";
        std::cout << _MeshData.AoTextureFilename << std::endl;

        std::wstring name = stringToWstring(_MeshData.AoTextureFilename);

        pMtrl->SetTexParam(TEX_1, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.NormalTextureFilename.empty())
    {
        std::wcout << _Mesh->GetName() << " : ";
        std::cout << _MeshData.NormalTextureFilename << std::endl;

        std::wstring name = stringToWstring(_MeshData.NormalTextureFilename);

        pMtrl->SetTexParam(TEX_2, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.HeightTextureFilename.empty())
    {
        std::wcout << _Mesh->GetName() << " : ";
        std::cout << _MeshData.HeightTextureFilename << std::endl;

        std::wstring name = stringToWstring(_MeshData.HeightTextureFilename);

        pMtrl->SetTexParam(TEX_3, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.MetallicTextureFilename.empty())
    {
        std::wcout << _Mesh->GetName() << " : ";
        std::cout << _MeshData.MetallicTextureFilename << std::endl;

        std::wstring name = stringToWstring(_MeshData.MetallicTextureFilename);

        pMtrl->SetTexParam(TEX_4, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.RoughnessTextureFilename.empty())
    {
        std::wcout << _Mesh->GetName() << " : ";
        std::cout << _MeshData.RoughnessTextureFilename << std::endl;

        std::wstring name = stringToWstring(_MeshData.RoughnessTextureFilename);

        // gLTF 포맷은 Metallic과 Roghness 를 한이미지에 같이 넣어사용함
        // 앞에서 Load한 MetallicTexture가 Roghness가 다른 텍스춰인경우에만 Load
        Ptr<CTexture> pTex = FindAsset<CTexture>(name);
        if (nullptr == pTex)
        {
            pMtrl->SetTexParam(TEX_4, Load<CTexture>(name, path + name));
        }
    }

    if (!_MeshData.EmissiveTextureFilename.empty())
    {
        std::wcout << _Mesh->GetName() << " : ";
        std::cout << _MeshData.EmissiveTextureFilename << std::endl;

        std::wstring name = stringToWstring(_MeshData.EmissiveTextureFilename);

        pMtrl->SetTexParam(TEX_5, Load<CTexture>(name, path + name));
    }

    return Ptr<CMaterial>(pMtrl);
}

CGameObject* CAssetMgr::LoadModel(const std::string& _relativepath, const std::string& _filename,
                                  const std::wstring& _name, bool revertNormals)
{
    auto meshes = ReadFromFile(_relativepath, _filename, revertNormals);

    CGameObject* model = new CGameObject;
    model->SetName(_name);
    model->AddComponent(new CTransform);

    int idx = 0;
    for (auto& meshData : meshes)
    {
        CGameObject* Parts = new CGameObject;
        Parts->SetName(_name + L" Parts " + std::to_wstring(idx));

        Ptr<CMesh> pMesh = new CMesh;
        pMesh->SetName(_name + L" Parts " + std::to_wstring(idx));
        pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(),
                      (UINT)meshData.indices.size());
        AddAsset<CMesh>(_name + L" Parts " + std::to_wstring(idx), pMesh);

        Ptr<CMaterial> material = LoadModelMaterial(pMesh, meshData);
        AddAsset<CMaterial>(_name + L" Parts " + std::to_wstring(idx) + L"Mtrl", material.Get());

        Parts->AddComponent(new CTransform);
        Parts->AddComponent(new CMeshRender);

        Parts->Transform()->SetAbsolute(false);
        Parts->MeshRender()->SetMesh(pMesh);
        Parts->MeshRender()->SetMaterial(material);

        model->AddChild(Parts);
        ++idx;
    }

    return model;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat,
                                       UINT _BindFlag, D3D11_USAGE _Usage)
{
    Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);

    assert(nullptr == pTex);

    pTex = new CTexture();
    if (FAILED(pTex->Create(_Width, _Height, _pixelformat, _BindFlag, _Usage)))
    {
        assert(nullptr);
    }

    AddAsset<CTexture>(_strKey, pTex);

    return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
    Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);

    assert(nullptr == pTex);

    pTex = new CTexture();
    if (FAILED(pTex->Create(_Tex2D)))
    {
        assert(nullptr);
    }

    pTex->SetKey(_strKey);
    AddAsset<CTexture>(_strKey, pTex);

    return pTex;
}

void CAssetMgr::DeleteAsset(ASSET_TYPE _type, const wstring& _strKey)
{
    map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)_type].find(_strKey);

    assert(!(iter == m_mapAsset[(UINT)_type].end()));

    m_mapAsset[(UINT)_type].erase(iter);
}
