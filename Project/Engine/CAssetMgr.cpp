#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CModleLoader.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h"
#include "COutputLog.h"

CAssetMgr::CAssetMgr()
{
}

CAssetMgr::~CAssetMgr()
{
    //// 모든 에셋 파일로 저장
    //for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
    //{
    //    wstring basePath = L"";

    //    switch ((ASSET_TYPE)i)
    //    {
    //    case ASSET_TYPE::MESH:
    //        break;
    //    case ASSET_TYPE::MESHDATA:
    //        break;
    //    case ASSET_TYPE::TEXTURE:
    //        basePath = L"Textures\\";
    //        break;
    //    case ASSET_TYPE::COMPUTE_SHADER:
    //        break;
    //    case ASSET_TYPE::GRAPHICS_SHADER:
    //        break;
    //    case ASSET_TYPE::MATERIAL:
    //        basePath = L"Materials\\";
    //        break;
    //    case ASSET_TYPE::SOUND:
    //        break;
    //    }

    //    if (basePath.empty())
    //        continue;

    //    for (const auto& iter : m_mapAsset[i])
    //    {
    //        wstring filePath = basePath;
    //        filePath += iter.first;
    //        filePath += m_AssetExtension;
    //        iter.second->Save(filePath);
    //    }
    //}
}

void CAssetMgr::init()
{
    m_AssetExtension = L".tasset";

    CreateDefaultMesh();
    CreateDefaultGraphicsShader();
    CreateDefaultComputeShader();
    CreateDefaultTexture();

    LoadFromAssetFile(); // 추후에 Mesh, Graphics Shader, Texture Save,Load 구현시 CreateDefault 위로 이동
    CreateDefaultMaterial();
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
                Load<CMaterial>(path.stem(), L"Materials\\" + wstring(path.filename()));
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

vector<tMeshData> CAssetMgr::ReadFromFile(string basePath, string filename, bool revertNormals)
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
    wstring path = ToWstring(_MeshData.RelativeTextureFilePath);

    if (!_MeshData.AmbientTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.AmbientTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.AmbientTextureFilename);

        pMtrl->SetTexParam(TEX_0, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.AoTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.AoTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.AoTextureFilename);

        pMtrl->SetTexParam(TEX_1, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.NormalTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.NormalTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.NormalTextureFilename);

        pMtrl->SetTexParam(TEX_2, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.HeightTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.HeightTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.HeightTextureFilename);

        pMtrl->SetTexParam(TEX_3, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.MetallicTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.MetallicTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.MetallicTextureFilename);

        pMtrl->SetTexParam(TEX_4, Load<CTexture>(name, path + name));
    }

    if (!_MeshData.RoughnessTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.RoughnessTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.RoughnessTextureFilename);

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
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.EmissiveTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.EmissiveTextureFilename);

        pMtrl->SetTexParam(TEX_5, Load<CTexture>(name, path + name));
    }

    return Ptr<CMaterial>(pMtrl);
}

CGameObject* CAssetMgr::LoadModel(const string& _relativepath, const string& _filename, const wstring& _name,
                                  bool revertNormals)
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
        pMesh->SetName(_name + L" Parts " + std::to_wstring(idx) + L" Mesh");
        pMesh->Create(meshData.vertices.data(), (UINT)meshData.vertices.size(), meshData.indices.data(),
                      (UINT)meshData.indices.size());
        AddAsset<CMesh>(pMesh->GetName(), pMesh);

        Ptr<CMaterial> material = LoadModelMaterial(pMesh, meshData);
        material->SetName(_name + L" Parts " + std::to_wstring(idx) + L" Mtrl");
        AddAsset<CMaterial>(material->GetName(), material.Get());

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
