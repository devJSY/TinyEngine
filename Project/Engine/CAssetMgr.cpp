#include "pch.h"
#include "CAssetMgr.h"
#include "CPathMgr.h"
#include "CTaskMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h"
#include "CSound.h"

CAssetMgr::CAssetMgr()
    : m_mapAsset{}
    , m_listLoadThread{}
    , m_Mutex()
    , m_CompletedThread(0)
    , m_vecLayoutInfo{}
    , m_iLayoutOffset_0(0)
    , m_iLayoutOffset_1(0)
{
}

CAssetMgr::~CAssetMgr()
{
    // Asset Release
    for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
    {
        m_mapAsset[i].clear();
    }

    // Sound Release
    if (nullptr != CSound::g_pFMOD)
    {
        CSound::g_pFMOD->release();
        CSound::g_pFMOD = nullptr;
    }
}

void CAssetMgr::init()
{
    CreateDefaultMesh();
    CreateDefaultGraphicsShader();
    CreateDefaultComputeShader();
    CreateDefaultMaterial();
    CreateDefaultPhysics2DMaterial();
    CreateDefaultPhysicMaterial();

    initSound();
}

void CAssetMgr::initSound()
{
    FMOD::System_Create(&CSound::g_pFMOD);

    if (nullptr == CSound::g_pFMOD)
    {
        assert(nullptr);
        return;
    }

    // 32�� ä�� ����
    CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
    CSound::g_pFMOD->set3DSettings(1.f, 1.f, 1.f); // Default
}

void CAssetMgr::tick()
{
    // �� �ε��� ����� �� ������ ����
    if (!IsAssetLoading())
    {
        ThreadRelease();
    }
}

void CAssetMgr::AsyncReloadContent()
{
    m_listLoadThread.push_back(std::thread(&CAssetMgr::AsyncReloadContentFunc, this));
}

void CAssetMgr::AsyncReloadContentFunc()
{
    std::scoped_lock lock(m_Mutex); // ��ȣ����

    LoadAssetsFromFile(CPathMgr::GetContentPath());

    // ���� ������ ������ ������ �޸𸮿��� ����
    for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
    {
        for (const auto& pair : m_mapAsset[i])
        {
            if (pair.second->IsEngineAsset())
                continue;

            wstring strFilePath = CPathMgr::GetContentPath() + pair.first;
            if (!std::filesystem::exists(strFilePath))
            {
                LOG(Warning, "%s Source File Deleted!", ToString(strFilePath).c_str());

                GamePlayStatic::DeleteAsset((ASSET_TYPE)i, pair.second);
            }
        }
    }

    ++m_CompletedThread;
}

void CAssetMgr::ThreadRelease()
{
    if (m_listLoadThread.empty())
        return;

    // �� Thread�� ����ɶ� ���� ���
    for (std::thread& Thread : m_listLoadThread)
    {
        if (Thread.joinable())
        {
            Thread.join();
        }
    }

    m_listLoadThread.clear();
    m_CompletedThread = 0;
}

void CAssetMgr::SaveAssetsToFile()
{
    for (UINT i = 0; i < (UINT)ASSET_TYPE::END; i++)
    {
        // Ư�� ������ ���� X
        if (i == (UINT)ASSET_TYPE::MESH || i == (UINT)ASSET_TYPE::TEXTURE)
            continue;

        for (const auto& pair : m_mapAsset[i])
        {
            pair.second->Save(pair.first);
        }
    }
}

void CAssetMgr::LoadAssetsFromFile(std::filesystem::path _EntryPath)
{
    using namespace std::filesystem;

    for (auto& directoryEntry : directory_iterator(_EntryPath))
    {
        // ���丮 �ΰ�� ���� ���丮 ��ȸ
        if (directoryEntry.is_directory())
        {
            LoadAssetsFromFile(directoryEntry);
        }
        else
        {
            path FilePath = directoryEntry.path();
            path FileRelativePath = FilePath.lexically_relative(CPathMgr::GetContentPath());
            path FileExtension = FilePath.extension();

            if (FileExtension == L".mesh")
                Load<CMesh>(FileRelativePath, FileRelativePath);
            if (FileExtension == L".mdat")
                Load<CMeshData>(FileRelativePath, FileRelativePath);
            if (FileExtension == L".pref")
                Load<CPrefab>(FileRelativePath, FileRelativePath);
            if (FileExtension == L".dds" || FileExtension == L".DDS" || FileExtension == L".tga" || FileExtension == L".TGA" ||
                FileExtension == L".png" || FileExtension == L".PNG" || FileExtension == L".bmp" || FileExtension == L".BMP" ||
                FileExtension == L".jpg" || FileExtension == L".JPG" || FileExtension == L".jpeg" || FileExtension == L".JPEG")
                Load<CTexture>(FileRelativePath, FileRelativePath);
            if (FileExtension == L".mtrl")
                Load<CMaterial>(FileRelativePath, FileRelativePath);
            if (FileExtension == L".wav" || FileExtension == L".mp3" || FileExtension == L".ogg")
                Load<CSound>(FileRelativePath, FileRelativePath);
            if (FileExtension == L".physic2Dmtrl")
                Load<CPhysics2DMaterial>(FileRelativePath, FileRelativePath);
            if (FileExtension == L".physicMaterial")
                Load<CPhysicMaterial>(FileRelativePath, FileRelativePath);
        }
    }
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat, UINT _BindFlag,
                                       D3D11_USAGE _Usage, const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc,
                                       const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc, const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc,
                                       const D3D11_UNORDERED_ACCESS_VIEW_DESC* _uavDesc)
{
    Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);

    if (nullptr != pTex)
    {
        assert(nullptr);
        return nullptr;
    }

    pTex = new CTexture(true);
    if (FAILED(pTex->Create(_Width, _Height, _pixelformat, _BindFlag, _Usage, _dsvDesc, _rtvDesc, _srvDesc, _uavDesc)))
    {
        assert(nullptr);
        return nullptr;
    }

    AddAsset<CTexture>(_strKey, pTex);

    return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D, const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc,
                                       const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc, const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc,
                                       const D3D11_UNORDERED_ACCESS_VIEW_DESC* _uavDesc)
{
    Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);

    if (nullptr != pTex)
    {
        assert(nullptr);
        return nullptr;
    }

    pTex = new CTexture(true);
    if (FAILED(pTex->Create(_Tex2D, _dsvDesc, _rtvDesc, _srvDesc, _uavDesc)))
    {
        assert(nullptr);
        return nullptr;
    }

    pTex->SetKey(_strKey);
    AddAsset<CTexture>(_strKey, pTex);

    return pTex;
}

void CAssetMgr::ReplacePrefab(Ptr<CPrefab> _Prefab, const wstring& _strKey)
{
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::PREFAB, _strKey);
    CAssetMgr::GetInst()->AddAsset<CPrefab>(_strKey, _Prefab);
    LOG(Log, "Prefab is Replaced!");
}

void CAssetMgr::DeleteAsset(ASSET_TYPE _type, const wstring& _strKey)
{
    map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)_type].find(_strKey);

    // �������� �ʴ� ������ �����Ϸ��� �� ���
    if (iter == m_mapAsset[(UINT)_type].end())
        return;

    m_mapAsset[(UINT)_type].erase(iter);
}
