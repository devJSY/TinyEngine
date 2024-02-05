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
    //        basePath = L"Meshes\\";
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
            basePath += L"Meshes\\";
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
                Load<CMesh>(L"Meshes\\" + wstring(path.filename()), L"Meshes\\" + wstring(path.filename()));
                break;
            case ASSET_TYPE::MESHDATA:
                break;
            case ASSET_TYPE::TEXTURE:
                break;
            case ASSET_TYPE::MATERIAL:
                Load<CMaterial>(L"Materials\\" + wstring(path.filename()), L"Materials\\" + wstring(path.filename()));
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

vector<tMeshData> CAssetMgr::ReadFromFile(string _basePath, string _filename, bool _revertNormals)
{
    CModelLoader modelLoader;
    modelLoader.Load(_basePath, _filename, _revertNormals);
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
    pMtrl->SetShader(FindAsset<CGraphicsShader>(L"UnrealPBRShader"));

    pMtrl->SetTexParam(TEXCUBE_0,
                       FindAsset<CTexture>(L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerSpecularHDR.dds"));
    pMtrl->SetTexParam(TEXCUBE_1,
                       FindAsset<CTexture>(L"Developers\\Textures\\Cubemaps\\IBLBaker\\IBLBakerDiffuseHDR.dds"));

    pMtrl->SetScalarParam(VEC4_0, Vec4(0.f, 0.f, 0.f, 1.f)); // Mtrl Albedo

    pMtrl->SetScalarParam(FLOAT_0, 1.f); // HeightScale
    pMtrl->SetScalarParam(FLOAT_1, 1.f); // Mtrl Metallic
    pMtrl->SetScalarParam(FLOAT_2, 1.f); // Mtrl Roughness

    pMtrl->SetScalarParam(INT_0, 0); // Invert NormalMap Y

    // 텍스쳐 로딩
    wstring path = ToWstring(_MeshData.RelativeTextureFilePath);

    if (!_MeshData.AmbientTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.AmbientTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.AmbientTextureFilename);

        pMtrl->SetTexParam(TEX_0, Load<CTexture>(path + name, path + name));
    }

    if (!_MeshData.AoTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.AoTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.AoTextureFilename);

        pMtrl->SetTexParam(TEX_1, Load<CTexture>(path + name, path + name));
    }

    if (!_MeshData.NormalTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.NormalTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.NormalTextureFilename);

        pMtrl->SetTexParam(TEX_2, Load<CTexture>(path + name, path + name));
    }

    if (!_MeshData.HeightTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.HeightTextureFilename.c_str());

        wstring name = ToWstring(_MeshData.HeightTextureFilename);

        pMtrl->SetTexParam(TEX_3, Load<CTexture>(path + name, path + name));
    }

    if (!_MeshData.MetallicTextureFilename.empty() || !_MeshData.RoughnessTextureFilename.empty())
    {
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.MetallicTextureFilename.c_str());
        LOG(Log, "%s : %s", ToString(_Mesh->GetName()).c_str(), _MeshData.RoughnessTextureFilename.c_str());
        wstring name = ToWstring(_MeshData.MetallicTextureFilename); // Metallic 이름으로 설정

        // GLTF 방식은 이미 합쳐져있음
        if (!_MeshData.MetallicTextureFilename.empty() &&
            (_MeshData.MetallicTextureFilename == _MeshData.RoughnessTextureFilename))
        {
            pMtrl->SetTexParam(TEX_4, Load<CTexture>(path + name, path + name));
        }
        else
        {
            // 별도 파일일 경우 따로 읽어서 합쳐줍니다.

            // ReadImage()를 활용하기 위해서 두 이미지들을 각각 4채널로 변환 후 다시
            // 3채널로 합치는 방식으로 구현
            int mWidth = 0, mHeight = 0;
            int rWidth = 0, rHeight = 0;
            std::vector<uint8_t> mImage;
            std::vector<uint8_t> rImage;

            // 둘 중 하나만 있을 경우도 고려하기 위해 각각 파일명 확인
            if (!_MeshData.MetallicTextureFilename.empty())
            {
                ReadImage(ToString(path) + _MeshData.MetallicTextureFilename, mImage, mWidth, mHeight);
            }

            if (!_MeshData.RoughnessTextureFilename.empty())
            {
                ReadImage(ToString(path) + _MeshData.RoughnessTextureFilename, rImage, rWidth, rHeight);
            }

            // 두 이미지의 해상도가 같다고 가정
            if (!_MeshData.MetallicTextureFilename.empty() && !_MeshData.RoughnessTextureFilename.empty())
            {
                assert(mWidth == rWidth);
                assert(mHeight == rHeight);
            }

            vector<uint8_t> combinedImage(size_t(mWidth * mHeight) * 4);
            fill(combinedImage.begin(), combinedImage.end(), 0);

            for (size_t i = 0; i < size_t(mWidth * mHeight); i++)
            {
                if (rImage.size())
                    combinedImage[4 * i + 1] = rImage[4 * i]; // Green = Roughness
                if (mImage.size())
                    combinedImage[4 * i + 2] = mImage[4 * i]; // Blue = Metalness
            }

            // 스테이징 텍스춰 만들고 CPU에서 이미지를 복사합니다.
            ComPtr<ID3D11Texture2D> stagingTexture =
                CreateStagingTexture(mWidth, mHeight, combinedImage, DXGI_FORMAT_R8G8B8A8_UNORM);

            // 실제로 사용할 텍스춰 설정
            ComPtr<ID3D11Texture2D> metallicRoughnessTexture;
            D3D11_TEXTURE2D_DESC txtDesc;
            ZeroMemory(&txtDesc, sizeof(txtDesc));
            txtDesc.Width = mWidth;
            txtDesc.Height = mHeight;
            txtDesc.MipLevels = 0; // 밉맵 레벨 최대
            txtDesc.ArraySize = 1;
            txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            txtDesc.SampleDesc.Count = 1;
            txtDesc.Usage = D3D11_USAGE_DEFAULT; // 스테이징 텍스춰로부터 복사 가능
            txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            txtDesc.CPUAccessFlags = 0;

            // 초기 데이터 없이 텍스춰 생성 (전부 검은색)
            DEVICE->CreateTexture2D(&txtDesc, NULL, metallicRoughnessTexture.GetAddressOf());

            // 전체 복사
            CONTEXT->CopyResource(metallicRoughnessTexture.Get(), stagingTexture.Get());

            pMtrl->SetTexParam(TEX_4, CreateTexture(path + name, metallicRoughnessTexture));
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

CGameObject* CAssetMgr::LoadModel(const wstring& _name, string _basePath, string _filename, bool _revertNormals,
                                  tMeshData _TexturesName)
{
    // 매쉬 로딩
    auto meshes = ReadFromFile(_basePath, _filename, _revertNormals);

    // .FBX 포맷일때는 텍스춰/경로 설정
    std::filesystem::path format = _filename;
    if (format.extension() == ".FBX" || format.extension() == ".fbx")
    {
        if (meshes[0].AmbientTextureFilename.empty())
            meshes[0].AmbientTextureFilename = _TexturesName.AmbientTextureFilename;

        if (meshes[0].AoTextureFilename.empty())
            meshes[0].AoTextureFilename = _TexturesName.AoTextureFilename;

        if (meshes[0].NormalTextureFilename.empty())
            meshes[0].NormalTextureFilename = _TexturesName.NormalTextureFilename;

        if (meshes[0].HeightTextureFilename.empty())
            meshes[0].HeightTextureFilename = _TexturesName.HeightTextureFilename;

        if (meshes[0].MetallicTextureFilename.empty())
            meshes[0].MetallicTextureFilename = _TexturesName.MetallicTextureFilename;

        if (meshes[0].RoughnessTextureFilename.empty())
            meshes[0].RoughnessTextureFilename = _TexturesName.RoughnessTextureFilename;

        if (meshes[0].EmissiveTextureFilename.empty())
            meshes[0].EmissiveTextureFilename = _TexturesName.EmissiveTextureFilename;

        if (meshes[0].RelativeTextureFilePath.empty())
            meshes[0].RelativeTextureFilePath = _TexturesName.RelativeTextureFilePath;
    }

    return LoadModel(_name, meshes);
}

CGameObject* CAssetMgr::LoadModel(const wstring& _name, vector<tMeshData> meshes)
{
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
