#include "pch.h"
#include "CMeshData.h"

#include "CPathMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAnimator3D.h"

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
    CGameObject* pNewObj = new CGameObject;
    pNewObj->AddComponent(new CTransform);
    pNewObj->AddComponent(new CMeshRender);

    pNewObj->MeshRender()->SetMesh(m_pMesh);

    for (UINT i = 0; i < m_vecMtrl.size(); ++i)
    {
        pNewObj->MeshRender()->SetMaterial(m_vecMtrl[i], i);
    }

    // Animation 파트 추가
    if (false == m_pMesh->IsAnimMesh())
        return pNewObj;

    CAnimator3D* pAnimator = new CAnimator3D;
    pNewObj->AddComponent(pAnimator);

    pAnimator->SetBones(m_pMesh->GetBones());
    pAnimator->SetAnimClip(m_pMesh->GetAnimClip());

    return pNewObj;
}

int CMeshData::Save(const wstring& _strRelativePath)
{
    SetRelativePath(_strRelativePath);

    wstring strFilePath = CPathMgr::GetContentPath() + _strRelativePath;

    FILE* pFile = nullptr;
    errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
    assert(pFile);

    // Mesh Key 저장
    // Mesh Data 저장
    SaveAssetRef(m_pMesh, pFile);

    // material 정보 저장
    UINT iMtrlCount = (UINT)m_vecMtrl.size();
    fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);

    UINT i = 0;
    wstring strMtrlPath = CPathMgr::GetContentPath();
    strMtrlPath += L"material\\";

    for (; i < iMtrlCount; ++i)
    {
        if (nullptr == m_vecMtrl[i])
            continue;

        // Material 인덱스, Key, Path 저장
        fwrite(&i, sizeof(UINT), 1, pFile);
        SaveAssetRef(m_vecMtrl[i], pFile);
    }

    i = -1; // 마감 값
    fwrite(&i, sizeof(UINT), 1, pFile);

    fclose(pFile);

    return S_OK;
}

int CMeshData::Load(const wstring& _strFilePath)
{
    FILE* pFile = NULL;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    assert(pFile);

    // Mesh Load
    LoadAssetRef<CMesh>(m_pMesh, pFile);
    assert(m_pMesh.Get());

    // material 정보 읽기
    UINT iMtrlCount = 0;
    fread(&iMtrlCount, sizeof(UINT), 1, pFile);

    m_vecMtrl.resize(iMtrlCount);

    for (UINT i = 0; i < iMtrlCount; ++i)
    {
        UINT idx = -1;
        fread(&idx, 4, 1, pFile);
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
    wstring strFullPath = CPathMgr::GetContentPath();
    strFullPath += _RelativePath;

    CFBXLoader loader;
    loader.init();
    loader.LoadFbx(strFullPath);

    // 메쉬 가져오기
    CMesh* pMesh = nullptr;
    pMesh = CMesh::CreateFromContainer(loader);

    // AssetMgr 에 메쉬 등록
    if (nullptr != pMesh)
    {
        wstring strMeshKey = L"mesh\\";
        strMeshKey += std::filesystem::path(strFullPath).stem();
        strMeshKey += L".mesh";
        CAssetMgr::GetInst()->AddAsset<CMesh>(strMeshKey, pMesh);

        // 메시를 실제 파일로 저장
        pMesh->Save(strMeshKey);
    }

    vector<Ptr<CMaterial>> vecMtrl;

    // 메테리얼 가져오기
    for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
    {
        // 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
        Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(loader.GetContainer(0).vecMtrl[i].strMtrlName);
        assert(pMtrl.Get());

        vecMtrl.push_back(pMtrl);
    }

    CMeshData* pMeshData = new CMeshData(true);
    pMeshData->m_pMesh = pMesh;
    pMeshData->m_vecMtrl = vecMtrl;

    return pMeshData;
}
