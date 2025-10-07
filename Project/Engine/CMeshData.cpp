#include "pch.h"
#include "CMeshData.h"

#include "CPathMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CGameObjectEx.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAnimator.h"

#include "CFBXLoader.h"

#include "CMesh.h"
#include "CMaterial.h"

CMeshData::CMeshData(bool _Engine)
    : CAsset(ASSET_TYPE::MESHDATA, _Engine)
{
}

CMeshData::~CMeshData()
{
}

CGameObject* CMeshData::Instantiate()
{
    CGameObject* pInstObj = new CGameObject;
    pInstObj->SetName(GetName());
    pInstObj->AddComponent(new CTransform);
    pInstObj->AddComponent(new CMeshRender);

    pInstObj->MeshRender()->SetMeshData(this);

    // Animation ��Ʈ �߰�
    if (false == m_pMesh->IsAnimMesh())
        return pInstObj;

    CAnimator* pAnimator = new CAnimator;
    pInstObj->AddComponent(pAnimator);

    pAnimator->SetSkeletalMesh(m_pMesh);

    return pInstObj;
}

CGameObjectEx* CMeshData::InstantiateEx()
{
    CGameObjectEx* pInstObjEx = new CGameObjectEx;
    pInstObjEx->SetName(GetName());
    pInstObjEx->AddComponent(new CTransform);
    pInstObjEx->AddComponent(new CMeshRender);

    pInstObjEx->MeshRender()->SetMeshData(this);

    // Animation ��Ʈ �߰�
    if (false == m_pMesh->IsAnimMesh())
        return pInstObjEx;

    CAnimator* pAnimator = new CAnimator;
    pInstObjEx->AddComponent(pAnimator);

    pAnimator->SetSkeletalMesh(m_pMesh);

    return pInstObjEx;
}

int CMeshData::Save(const wstring& _strRelativePath)
{
    if (IsEngineAsset())
        return E_FAIL;

    SetRelativePath(_strRelativePath);

    wstring strFilePath = CPathMgr::GetContentPath() + _strRelativePath;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    // Mesh Key ����
    // Mesh Data ����
    SaveAssetRef(m_pMesh, pFile);

    // material ���� ����
    UINT iMtrlCount = (UINT)m_vecMtrl.size();
    fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

    UINT i = 0;
    wstring strMtrlPath = CPathMgr::GetContentPath();
    strMtrlPath += L"material/";

    for (; i < iMtrlCount; ++i)
    {
        if (nullptr == m_vecMtrl[i])
            continue;

        // Material �ε���, Key, Path ����
        fwrite(&i, sizeof(UINT), 1, pFile);
        SaveAssetRef(m_vecMtrl[i], pFile);
    }

    i = -1; // ���� ��
    fwrite(&i, sizeof(UINT), 1, pFile);

    fclose(pFile);

    return S_OK;
}

int CMeshData::Load(const wstring& _strFilePath)
{
    FILE* pFile = NULL;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    if (nullptr == pFile)
    {
        MessageBox(nullptr, L"MeshData �ε� ����", L"MeshData �ε� ����", MB_OK);
        return E_FAIL;
    }

    // Mesh Load
    LoadAssetRef<CMesh>(m_pMesh, pFile);

    if (nullptr == m_pMesh)
        return E_FAIL;

    // material ���� �б�
    UINT iMtrlCount = 0;
    fread(&iMtrlCount, sizeof(UINT), 1, pFile);

    m_vecMtrl.resize(iMtrlCount);

    for (UINT i = 0; i < iMtrlCount; ++i)
    {
        UINT idx = -1;
        fread(&idx, sizeof(UINT), 1, pFile);
        if (idx == -1)
            break;

        wstring strKey, strPath;

        Ptr<CMaterial> pMtrl;
        LoadAssetRef<CMaterial>(pMtrl, pFile);

        m_vecMtrl[i] = pMtrl;
    }

    fclose(pFile);

    return S_OK;
}

CMeshData* CMeshData::LoadFromFBX(const wstring& _RelativePath)
{
    CFBXLoader loader;
    loader.init();
    loader.LoadFbx(_RelativePath);

    // �޽� ��������
    CMesh* pMesh = nullptr;
    pMesh = CMesh::CreateFromContainer(loader);

    // AssetMgr �� �޽� ���
    if (nullptr != pMesh)
    {
        wstring strMeshKey = L"mesh/";
        strMeshKey += std::filesystem::path(_RelativePath).stem();
        strMeshKey += L".mesh";
        CAssetMgr::GetInst()->AddAsset<CMesh>(strMeshKey, pMesh);

        // �޽ø� ���� ����
        pMesh->Save(strMeshKey);
    }

    vector<Ptr<CMaterial>> vecMtrl;

    // ���׸��� ��������
    for (UINT i = 0; i < (UINT)loader.GetContainerCount(); ++i)
    {
        for (UINT j = 0; j < (UINT)loader.GetContainer(i).vecMtrl.size(); ++j)
        {
            // ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
            Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(loader.GetContainer(i).vecMtrl[j].strMtrlName);
            if (nullptr == pMtrl)
                continue;

            vecMtrl.push_back(pMtrl);
            pMtrl->Save(pMtrl->GetKey());
        }
    }

    CMeshData* pMeshData = new CMeshData();
    pMeshData->m_pMesh = pMesh;
    pMeshData->m_vecMtrl = vecMtrl;

    return pMeshData;
}
