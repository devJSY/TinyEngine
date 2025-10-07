#include "pch.h"
#include "CFBXLoader.h"
#include "CMesh.h"

#include "CAssetMgr.h"
#include "CPathMgr.h"

CFBXLoader::CFBXLoader()
    : m_pManager(NULL)
    , m_pScene(NULL)
    , m_pImporter(NULL)
{
}

CFBXLoader::~CFBXLoader()
{
    m_pScene->Destroy();
    m_pManager->Destroy();

    for (size_t i = 0; i < m_vecBone.size(); ++i)
    {
        if (nullptr != m_vecBone[i])
            delete m_vecBone[i];
    }

    for (size_t i = 0; i < m_vecAnimClip.size(); ++i)
    {
        if (nullptr != m_vecAnimClip[i])
            delete m_vecAnimClip[i];
    }

    for (int i = 0; i < m_arrAnimName.Size(); ++i)
    {
        if (nullptr != m_arrAnimName[i])
            delete m_arrAnimName[i];
    }
}

void CFBXLoader::init()
{
    m_pManager = FbxManager::Create();
    if (NULL == m_pManager)
        assert(NULL);

    FbxIOSettings* pIOSettings = FbxIOSettings::Create(m_pManager, IOSROOT);
    m_pManager->SetIOSettings(pIOSettings);

    m_pScene = FbxScene::Create(m_pManager, "");
    if (NULL == m_pScene)
        assert(NULL);
}

void CFBXLoader::LoadFbx(const wstring& _RelativePath)
{
    wstring strFullPath = CPathMgr::GetContentPath();
    strFullPath += _RelativePath;

    m_vecContainer.clear();

    m_pImporter = FbxImporter::Create(m_pManager, "");

    if (!m_pImporter->Initialize(ToString(strFullPath).c_str(), -1, m_pManager->GetIOSettings()))
        assert(nullptr);

    m_pImporter->Import(m_pScene);

    if (m_pScene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
    {
        m_pScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);
    }

    // Bone ���� �б�
    LoadSkeleton(m_pScene->GetRootNode());

    // Animation �̸�����
    m_pScene->FillAnimStackNameArray(m_arrAnimName);

    // Animation Clip ����
    LoadAnimationClip();

    // �ﰢȭ(Triangulate)
    Triangulate(m_pScene->GetRootNode());

    // �޽� ������ ���
    LoadMeshDataFromNode(m_pScene->GetRootNode());

    m_pImporter->Destroy();

    // �ʿ��� �ؽ��� �ε�
    LoadTexture(_RelativePath);

    // �ʿ��� ���׸��� ����
    CreateMaterial(_RelativePath);
}

void CFBXLoader::LoadMeshDataFromNode(FbxNode* _pNode)
{
    // ����� �޽����� �б�
    FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

    if (pAttr && FbxNodeAttribute::eMesh == pAttr->GetAttributeType())
    {
        FbxMesh* pMesh = _pNode->GetMesh();
        if (NULL != pMesh)
            LoadMesh(pMesh);
    }

    // �ش� ����� �������� �б�
    UINT iMtrlCnt = _pNode->GetMaterialCount();
    if (iMtrlCnt > 0)
    {
        for (UINT i = 0; i < iMtrlCnt; ++i)
        {
            FbxSurfaceMaterial* pMtrlSur = _pNode->GetMaterial(i);
            LoadMaterial(pMtrlSur);
        }
    }

    // �ڽ� ��� ���� �б�
    int iChildCnt = _pNode->GetChildCount();
    for (int i = 0; i < iChildCnt; ++i)
    {
        LoadMeshDataFromNode(_pNode->GetChild(i));
    }
}

void CFBXLoader::LoadMesh(FbxMesh* _pFbxMesh)
{
    m_vecContainer.push_back(tContainer{});
    tContainer& Container = m_vecContainer[m_vecContainer.size() - 1];

    string strName = _pFbxMesh->GetName();
    Container.strName = wstring(strName.begin(), strName.end());

    int iVtxCnt = _pFbxMesh->GetControlPointsCount();
    Container.Resize(iVtxCnt);

    FbxVector4* pFbxPos = _pFbxMesh->GetControlPoints();

    for (int i = 0; i < iVtxCnt; ++i)
    {
        Container.vecPos[i].x = (float)pFbxPos[i].mData[0];
        Container.vecPos[i].y = (float)pFbxPos[i].mData[2];
        Container.vecPos[i].z = (float)pFbxPos[i].mData[1];
    }

    // ������ ����
    int iPolyCnt = _pFbxMesh->GetPolygonCount();

    // ������ ���� ( ==> SubSet ���� ==> Index Buffer Count)
    int iMtrlCnt = _pFbxMesh->GetNode()->GetMaterialCount();
    Container.vecIdx.resize(iMtrlCnt);

    // ���� ������ ���� subset �� �˱����ؼ�...
    FbxGeometryElementMaterial* pMtrl = _pFbxMesh->GetElementMaterial();

    // �������� �����ϴ� ���� ����
    int iPolySize = _pFbxMesh->GetPolygonSize(0);

    if (3 != iPolySize)
        assert(NULL); // Polygon ���� ������ 3���� �ƴ� ���

    UINT arrIdx[3] = {};
    UINT iVtxOrder = 0; // ������ ������ �����ϴ� ����

    for (int i = 0; i < iPolyCnt; ++i)
    {
        for (int j = 0; j < iPolySize; ++j)
        {
            // i ��° �����￡, j ��° ����
            int iIdx = _pFbxMesh->GetPolygonVertex(i, j);
            arrIdx[j] = iIdx;

            // Normal �� �������� �ʴ� ��� ����
            if (0 == _pFbxMesh->GetElementNormalCount())
            {
                _pFbxMesh->InitNormals();
                _pFbxMesh->GenerateNormals();
            }
            // Tangent, Binormal�� �������� �ʴ� ��� ����
            if (0 == _pFbxMesh->GetElementTangentCount() || 0 == _pFbxMesh->GetElementBinormalCount())
            {
                _pFbxMesh->InitTangents();
                _pFbxMesh->InitBinormals();
                _pFbxMesh->GenerateTangentsData(NULL, true);
            }

            GetTangent(_pFbxMesh, &Container, iIdx, iVtxOrder);
            GetBinormal(_pFbxMesh, &Container, iIdx, iVtxOrder);
            GetNormal(_pFbxMesh, &Container, iIdx, iVtxOrder);

            GetColor(_pFbxMesh, &Container, iIdx, iVtxOrder);

            GetUV(_pFbxMesh, &Container, iIdx, _pFbxMesh->GetTextureUVIndex(i, j), iVtxOrder);
            
            ++iVtxOrder;
        }
        UINT iSubsetIdx = pMtrl->GetIndexArray().GetAt(i);
        Container.vecIdx[iSubsetIdx].push_back(arrIdx[0]);
        Container.vecIdx[iSubsetIdx].push_back(arrIdx[2]);
        Container.vecIdx[iSubsetIdx].push_back(arrIdx[1]);
    }

    LoadAnimationData(_pFbxMesh, &Container);
}

void CFBXLoader::LoadMaterial(FbxSurfaceMaterial* _pMtrlSur)
{
    tFbxMaterial tMtrlInfo{};

    string str = _pMtrlSur->GetName();
    tMtrlInfo.strMtrlName = ToWstring(str);

    tMtrlInfo.tMtrl.vAlbedo = GetMtrlData(_pMtrlSur, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
    tMtrlInfo.tMtrl.vDiffuse = GetMtrlData(_pMtrlSur, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
    tMtrlInfo.tMtrl.vSpecular = GetMtrlData(_pMtrlSur, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
    tMtrlInfo.tMtrl.vMetallic = GetMtrlData(_pMtrlSur, FbxSurfaceMaterial::sReflection, FbxSurfaceMaterial::sReflectionFactor).w;
    tMtrlInfo.tMtrl.vRoughness = GetMtrlData(_pMtrlSur, FbxSurfaceMaterial::sShininess, FbxSurfaceMaterial::sSpecularFactor).w;
    tMtrlInfo.tMtrl.vEmission = GetMtrlData(_pMtrlSur, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

    // Texture Name
    tMtrlInfo.strAlbedo = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sAmbient);
    tMtrlInfo.strNormal = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sNormalMap);
    tMtrlInfo.strMRA = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sReflection); // Metallic
    if (tMtrlInfo.strMRA.empty())
        tMtrlInfo.strMRA = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sShininess); // Roughness
    tMtrlInfo.strEmissive = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sEmissive);

    if (tMtrlInfo.strAlbedo.empty())
        tMtrlInfo.strAlbedo = GetMtrlTextureName(_pMtrlSur, FbxSurfaceMaterial::sDiffuse);

    m_vecContainer.back().vecMtrl.push_back(tMtrlInfo);
}

void CFBXLoader::GetTangent(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx /*�ش� ������ �ε���*/
                            ,
                            int _iVtxOrder /*������ ������ �����ϴ� ����*/)
{
    int iTangentCnt = _pMesh->GetElementTangentCount();
    if (1 != iTangentCnt)
        assert(NULL); // ���� 1���� �����ϴ� ź��Ʈ ������ 2�� �̻��̴�.

    // ź��Ʈ data �� ���� �ּ�
    FbxGeometryElementTangent* pTangent = _pMesh->GetElementTangent();
    UINT iTangentIdx = 0;

    if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
        if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
            iTangentIdx = _iVtxOrder;
        else
            iTangentIdx = pTangent->GetIndexArray().GetAt(_iVtxOrder);
    }
    else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
    {
        if (pTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
            iTangentIdx = _iIdx;
        else
            iTangentIdx = pTangent->GetIndexArray().GetAt(_iIdx);
    }

    FbxVector4 vTangent = pTangent->GetDirectArray().GetAt(iTangentIdx);

    _pContainer->vecTangent[_iIdx].x = (float)vTangent.mData[0];
    _pContainer->vecTangent[_iIdx].y = (float)vTangent.mData[2];
    _pContainer->vecTangent[_iIdx].z = (float)vTangent.mData[1];
}

void CFBXLoader::GetBinormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder)
{
    int iBinormalCnt = _pMesh->GetElementBinormalCount();
    if (1 != iBinormalCnt)
        assert(NULL); // ���� 1���� �����ϴ� ������ ������ 2�� �̻��̴�.

    // ������ data �� ���� �ּ�
    FbxGeometryElementBinormal* pBinormal = _pMesh->GetElementBinormal();
    UINT iBinormalIdx = 0;

    if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
        if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
            iBinormalIdx = _iVtxOrder;
        else
            iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iVtxOrder);
    }
    else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
    {
        if (pBinormal->GetReferenceMode() == FbxGeometryElement::eDirect)
            iBinormalIdx = _iIdx;
        else
            iBinormalIdx = pBinormal->GetIndexArray().GetAt(_iIdx);
    }

    FbxVector4 vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIdx);

    _pContainer->vecBinormal[_iIdx].x = (float)vBinormal.mData[0];
    _pContainer->vecBinormal[_iIdx].y = (float)vBinormal.mData[2];
    _pContainer->vecBinormal[_iIdx].z = (float)vBinormal.mData[1];
}

void CFBXLoader::GetNormal(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder)
{
    int iNormalCnt = _pMesh->GetElementNormalCount();
    if (1 != iNormalCnt)
        assert(NULL); // ���� 1���� �����ϴ� ������ ������ 2�� �̻��̴�.

    // ������ data �� ���� �ּ�
    FbxGeometryElementNormal* pNormal = _pMesh->GetElementNormal();
    UINT iNormalIdx = 0;

    if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
        if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
            iNormalIdx = _iVtxOrder;
        else
            iNormalIdx = pNormal->GetIndexArray().GetAt(_iVtxOrder);
    }
    else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
    {
        if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
            iNormalIdx = _iIdx;
        else
            iNormalIdx = pNormal->GetIndexArray().GetAt(_iIdx);
    }

    FbxVector4 vNormal = pNormal->GetDirectArray().GetAt(iNormalIdx);

    _pContainer->vecNormal[_iIdx].x = (float)vNormal.mData[0];
    _pContainer->vecNormal[_iIdx].y = (float)vNormal.mData[2];
    _pContainer->vecNormal[_iIdx].z = (float)vNormal.mData[1];
}

void CFBXLoader::GetColor(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iVtxOrder)
{
    // for (int i = 0; i < _pMesh->GetElementVertexColorCount(); i++)
    FbxGeometryElementVertexColor* leVtxc = _pMesh->GetElementVertexColor(0);

    if (!leVtxc)
        return;

    FbxColor Color = FbxColor();
    if (leVtxc->GetMappingMode() == FbxGeometryElement::eByControlPoint)
    {
        if (leVtxc->GetReferenceMode() == FbxGeometryElement::eDirect)
        {
            Color = leVtxc->GetDirectArray().GetAt(_iIdx);
        }
        else if (leVtxc->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
        {
            int id = leVtxc->GetIndexArray().GetAt(_iIdx);
            Color = leVtxc->GetDirectArray().GetAt(id);
        }
    }
    else if (leVtxc->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
    {
        if (leVtxc->GetReferenceMode() == FbxGeometryElement::eDirect)
        {
            Color = leVtxc->GetDirectArray().GetAt(_iVtxOrder);
        }
        else if (leVtxc->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
        {
            int id = leVtxc->GetIndexArray().GetAt(_iVtxOrder);
            Color = leVtxc->GetDirectArray().GetAt(id);
        }
    }

    _pContainer->vecColor[_iIdx].x = (float)Color.mRed;
    _pContainer->vecColor[_iIdx].y = (float)Color.mGreen;
    _pContainer->vecColor[_iIdx].z = (float)Color.mBlue;
    _pContainer->vecColor[_iIdx].w = (float)Color.mAlpha;
}

void CFBXLoader::GetUV(FbxMesh* _pMesh, tContainer* _pContainer, int _iIdx, int _iUVIndex, int _iVtxOrder)
{
    for (int ElementCount = 0; ElementCount < 4; ElementCount++)
    {
        const FbxGeometryElementUV* lUVElement = _pMesh->GetElementUV(ElementCount);
        
        if (!lUVElement)
            continue;

        // only support mapping mode eByPolygonVertex and eByControlPoint
        if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
            lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
            continue;

        // index array, where holds the index referenced to the uv data
        const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
        const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;
        FbxVector2 lUVValue;

        if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
        {
            int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(_iIdx) : _iIdx;
            lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
        }
        else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
            // the UV index depends on the reference mode
            int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(_iVtxOrder) : _iVtxOrder;
            lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
        }

        switch (ElementCount)
        {
        case 0: {
            _pContainer->vecUV0[_iIdx].x = (float)lUVValue.mData[0];
            _pContainer->vecUV0[_iIdx].y = 1.f - (float)lUVValue.mData[1]; // fbx uv ��ǥ��� ���ϴ��� 0,0
        }
        break;
        case 1: {
            _pContainer->vecUV1[_iIdx].x = (float)lUVValue.mData[0];
            _pContainer->vecUV1[_iIdx].y = 1.f - (float)lUVValue.mData[1]; // fbx uv ��ǥ��� ���ϴ��� 0,0
        }
        break;
        case 2: {
            _pContainer->vecUV2[_iIdx].x = (float)lUVValue.mData[0];
            _pContainer->vecUV2[_iIdx].y = 1.f - (float)lUVValue.mData[1]; // fbx uv ��ǥ��� ���ϴ��� 0,0
        }
        break;
        case 3: {
            _pContainer->vecUV3[_iIdx].x = (float)lUVValue.mData[0];
            _pContainer->vecUV3[_iIdx].y = 1.f - (float)lUVValue.mData[1]; // fbx uv ��ǥ��� ���ϴ��� 0,0
        }
        break;
        }
    }
}

Vec4 CFBXLoader::GetMtrlData(FbxSurfaceMaterial* _pSurface, const char* _pMtrlName, const char* _pMtrlFactorName)
{
    FbxDouble3 vMtrl;
    FbxDouble dFactor = 0.;

    FbxProperty tMtrlProperty = _pSurface->FindProperty(_pMtrlName);
    FbxProperty tMtrlFactorProperty = _pSurface->FindProperty(_pMtrlFactorName);

    if (tMtrlProperty.IsValid() && tMtrlFactorProperty.IsValid())
    {
        vMtrl = tMtrlProperty.Get<FbxDouble3>();
        dFactor = tMtrlFactorProperty.Get<FbxDouble>();
    }

    Vec4 vRetVal =
        Vec4((float)vMtrl.mData[0] * (float)dFactor, (float)vMtrl.mData[1] * (float)dFactor, (float)vMtrl.mData[2] * (float)dFactor, (float)dFactor);
    return vRetVal;
}

wstring CFBXLoader::GetMtrlTextureName(FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty)
{
    string strName;

    FbxProperty TextureProperty = _pSurface->FindProperty(_pMtrlProperty);
    if (TextureProperty.IsValid())
    {
        UINT iCnt = TextureProperty.GetSrcObjectCount();

        if (1 <= iCnt)
        {
            FbxFileTexture* pFbxTex = TextureProperty.GetSrcObject<FbxFileTexture>(0);
            if (NULL != pFbxTex)
                strName = pFbxTex->GetFileName();
        }
    }

    return wstring(strName.begin(), strName.end());
}

void CFBXLoader::LoadTexture(const wstring& _RelativePath)
{
    using namespace std::filesystem;

    for (UINT i = 0; i < m_vecContainer.size(); ++i)
    {
        for (UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j)
        {
            vector<path> vecPath;
            vecPath.push_back(m_vecContainer[i].vecMtrl[j].strAlbedo.c_str());
            vecPath.push_back(m_vecContainer[i].vecMtrl[j].strNormal.c_str());
            vecPath.push_back(m_vecContainer[i].vecMtrl[j].strMRA.c_str());
            vecPath.push_back(m_vecContainer[i].vecMtrl[j].strEmissive.c_str());

            for (size_t k = 0; k < vecPath.size(); ++k)
            {
                if (vecPath[k].filename().empty())
                    continue;

                // .fbx ������ �����ϴ� ����� Texture ���� Load
                wstring TexturePath = path(_RelativePath).parent_path();
                TexturePath += L"/";
                TexturePath += vecPath[k].filename();
                CAssetMgr::GetInst()->Load<CTexture>(TexturePath, TexturePath);

                switch (k)
                {
                case 0:
                    m_vecContainer[i].vecMtrl[j].strAlbedo = TexturePath;
                    break;
                case 1:
                    m_vecContainer[i].vecMtrl[j].strNormal = TexturePath;
                    break;
                case 2:
                    m_vecContainer[i].vecMtrl[j].strMRA = TexturePath;
                    break;
                case 3:
                    m_vecContainer[i].vecMtrl[j].strEmissive = TexturePath;
                    break;
                }
            }
        }
    }
}

void CFBXLoader::CreateMaterial(const wstring& _RelativePath)
{
    using namespace std::filesystem;

    wstring strMtrlFileName;
    wstring strMtrlName;
    wstring strPath;

    for (UINT i = 0; i < m_vecContainer.size(); ++i)
    {
        for (UINT j = 0; j < m_vecContainer[i].vecMtrl.size(); ++j)
        {
            // Material �̸�����
            strMtrlFileName = m_vecContainer[i].vecMtrl[j].strMtrlName;

            // FileName_
            strMtrlName = path(_RelativePath).stem();
            strMtrlName += L"_";

            // + Mtrl Name
            strMtrlName += strMtrlFileName;
            if (strMtrlFileName.empty())
                strMtrlName += path(m_vecContainer[i].vecMtrl[j].strAlbedo).stem();

            strPath = L"material/";
            strPath += strMtrlName + L".mtrl";

            // ���� �̸�
            m_vecContainer[i].vecMtrl[j].strMtrlName = strPath;

            // �̹� �ε��� �����̸� �ε��� ���� ���
            Ptr<CMaterial> pMaterial = CAssetMgr::GetInst()->FindAsset<CMaterial>(strPath);
            if (nullptr != pMaterial)
                continue;

            pMaterial = new CMaterial;

            // ����ΰ� �� Ű
            pMaterial->SetKey(strPath);
            pMaterial->SetRelativePath(strPath);

            pMaterial->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"UnrealPBRDeferredShader"));

            // Albedo
            wstring strTexKey = m_vecContainer[i].vecMtrl[j].strAlbedo;
            Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexKey);
            if (NULL != pTex)
                pMaterial->SetTexParam(TEX_PARAM::TEX_0, pTex);

            // Metallic, Roughness, Ambient Occlusion
            strTexKey = m_vecContainer[i].vecMtrl[j].strMRA;
            pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexKey);
            if (NULL != pTex)
                pMaterial->SetTexParam(TEX_PARAM::TEX_4, pTex);

            // Normal
            strTexKey = m_vecContainer[i].vecMtrl[j].strNormal;
            pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexKey);
            if (NULL != pTex)
                pMaterial->SetTexParam(TEX_PARAM::TEX_5, pTex);

            // Height
            // TEX_6

            // Emissive
            strTexKey = m_vecContainer[i].vecMtrl[j].strEmissive;
            pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexKey);
            if (NULL != pTex)
                pMaterial->SetTexParam(TEX_PARAM::TEX_7, pTex);

            pMaterial->SetMaterialCoefficient(m_vecContainer[i].vecMtrl[j].tMtrl.vAlbedo, m_vecContainer[i].vecMtrl[j].tMtrl.vDiffuse,
                                              m_vecContainer[i].vecMtrl[j].tMtrl.vSpecular, m_vecContainer[i].vecMtrl[j].tMtrl.vMetallic,
                                              m_vecContainer[i].vecMtrl[j].tMtrl.vRoughness, m_vecContainer[i].vecMtrl[j].tMtrl.vEmission);

            // Texture Parse
            wstring TexturePath = CPathMgr::GetContentPath();
            TexturePath += path(_RelativePath).parent_path();
            TexturePath += L"/";
            ParseTexture(TexturePath, strMtrlFileName, pMaterial);

            CAssetMgr::GetInst()->AddAsset<CMaterial>(pMaterial->GetKey(), pMaterial.Get());
        }
    }
}

void CFBXLoader::ParseTexture(std::filesystem::path _EntryPath, const wstring& _MtrlName, const Ptr<CMaterial>& _Mtrl)
{
    using namespace std::filesystem;

    for (auto& directoryEntry : directory_iterator(_EntryPath))
    {
        // ���丮 �ΰ�� ���� ���丮 ��ȸ
        if (directoryEntry.is_directory())
        {
            ParseTexture(directoryEntry, _MtrlName, _Mtrl);
        }
        else
        {
            path FilePath = directoryEntry.path();
            path FileRelativePath = FilePath.lexically_relative(CPathMgr::GetContentPath());
            path FileExtension = FilePath.extension();

            // .png ���ϸ� �ε�
            if (FileExtension != L".png")
                continue;

            wstring TexturePath = FilePath;
            FilePath = FilePath.parent_path();
            FilePath += L"/";

            // Albedo
            if (nullptr == _Mtrl->GetTexParam(TEX_0) && string::npos != TexturePath.find(wstring(FilePath) + _MtrlName + L"_BaseColor"))
            {
                _Mtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(FileRelativePath, FileRelativePath));
            }

            // MRA
            if (nullptr == _Mtrl->GetTexParam(TEX_4) && string::npos != TexturePath.find(wstring(FilePath) + _MtrlName + L"_MRA"))
            {
                _Mtrl->SetTexParam(TEX_4, CAssetMgr::GetInst()->Load<CTexture>(FileRelativePath, FileRelativePath));
            }

            // Normal
            if (nullptr == _Mtrl->GetTexParam(TEX_5) && string::npos != TexturePath.find(wstring(FilePath) + _MtrlName + L"_Normal"))
            {
                _Mtrl->SetTexParam(TEX_5, CAssetMgr::GetInst()->Load<CTexture>(FileRelativePath, FileRelativePath));
            }

            // Height
            if (nullptr == _Mtrl->GetTexParam(TEX_6) && string::npos != TexturePath.find(wstring(FilePath) + _MtrlName + L"_Height"))
            {
                _Mtrl->SetTexParam(TEX_6, CAssetMgr::GetInst()->Load<CTexture>(FileRelativePath, FileRelativePath));
            }

            // Emissive
            if (nullptr == _Mtrl->GetTexParam(TEX_7) && string::npos != TexturePath.find(wstring(FilePath) + _MtrlName + L"_Emissive"))
            {
                _Mtrl->SetTexParam(TEX_7, CAssetMgr::GetInst()->Load<CTexture>(FileRelativePath, FileRelativePath));
            }
        }
    }
}

void CFBXLoader::LoadSkeleton(FbxNode* _pNode)
{
    int iChildCount = _pNode->GetChildCount();

    LoadSkeleton_Re(_pNode, 0, 0, -1);
}

void CFBXLoader::LoadSkeleton_Re(FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx)
{
    FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

    if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
    {
        tBone* pBone = new tBone;

        string strBoneName = _pNode->GetName();

        pBone->strBoneName = wstring(strBoneName.begin(), strBoneName.end());
        pBone->iDepth = _iDepth++;
        pBone->iParentIndx = _iParentIdx;

        m_vecBone.push_back(pBone);
    }

    int iChildCount = _pNode->GetChildCount();
    for (int i = 0; i < iChildCount; ++i)
    {
        LoadSkeleton_Re(_pNode->GetChild(i), _iDepth, (int)m_vecBone.size(), _iIdx);
    }
}

void CFBXLoader::LoadAnimationClip()
{
    int iAnimCount = m_arrAnimName.GetCount();

    for (int i = 0; i < iAnimCount; ++i)
    {
        FbxAnimStack* pAnimStack = m_pScene->FindMember<FbxAnimStack>(m_arrAnimName[i]->Buffer());

        if (!pAnimStack)
            continue;

        tAnimClip* pAnimClip = new tAnimClip;

        string strClipName = pAnimStack->GetName();
        pAnimClip->strName = wstring(strClipName.begin(), strClipName.end());

        FbxTakeInfo* pTakeInfo = m_pScene->GetTakeInfo(pAnimStack->GetName());
        pAnimClip->tStartTime = pTakeInfo->mLocalTimeSpan.GetStart();
        pAnimClip->tEndTime = pTakeInfo->mLocalTimeSpan.GetStop();

        pAnimClip->eMode = m_pScene->GetGlobalSettings().GetTimeMode();
        pAnimClip->llTimeLength = pAnimClip->tEndTime.GetFrameCount(pAnimClip->eMode) - pAnimClip->tStartTime.GetFrameCount(pAnimClip->eMode);

        m_vecAnimClip.push_back(pAnimClip);
    }
}

void CFBXLoader::Triangulate(FbxNode* _pNode)
{
    FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

    if (pAttr && (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh || pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs ||
                  pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
    {
        FbxGeometryConverter converter(m_pManager);
        converter.Triangulate(pAttr, true);
    }

    int iChildCount = _pNode->GetChildCount();

    for (int i = 0; i < iChildCount; ++i)
    {
        Triangulate(_pNode->GetChild(i));
    }
}

void CFBXLoader::LoadAnimationData(FbxMesh* _pMesh, tContainer* _pContainer)
{
    // Animation Data �ε��� �ʿ䰡 ����
    int iSkinCount = _pMesh->GetDeformerCount(FbxDeformer::eSkin);
    if (iSkinCount <= 0 || m_vecAnimClip.empty())
        return;

    _pContainer->bAnimation = true;

    // Skin ������ŭ �ݺ����ϸ� �д´�.
    for (int i = 0; i < iSkinCount; ++i)
    {
        FbxSkin* pSkin = (FbxSkin*)_pMesh->GetDeformer(i, FbxDeformer::eSkin);

        if (pSkin)
        {
            FbxSkin::EType eType = pSkin->GetSkinningType();
            if (FbxSkin::eRigid == eType || FbxSkin::eLinear)
            {
                // Cluster �� ���´�
                // Cluster == Joint == ����
                int iClusterCount = pSkin->GetClusterCount();

                for (int j = 0; j < iClusterCount; ++j)
                {
                    FbxCluster* pCluster = pSkin->GetCluster(j);

                    if (!pCluster->GetLink())
                        continue;

                    // ���� �� �ε����� ���´�.
                    int iBoneIdx = FindBoneIndex(pCluster->GetLink()->GetName());
                    if (-1 == iBoneIdx)
                        assert(NULL);

                    FbxAMatrix matNodeTransform = GetTransform(_pMesh->GetNode());

                    // Weights And Indices ������ �д´�.
                    LoadWeightsAndIndices(pCluster, iBoneIdx, _pContainer);

                    // Bone �� OffSet ��� ���Ѵ�.
                    LoadOffsetMatrix(pCluster, matNodeTransform, iBoneIdx, _pContainer);

                    // Bone�� KeyFrame ����� ���Ѵ�.
                    LoadKeyframeTransform(_pMesh->GetNode(), pCluster, matNodeTransform, iBoneIdx, _pContainer);
                }
            }
        }
    }
    CheckWeightAndIndices(_pMesh, _pContainer);
}

void CFBXLoader::CheckWeightAndIndices(FbxMesh* _pMesh, tContainer* _pContainer)
{
    vector<vector<tWeightsAndIndices>>::iterator iter = _pContainer->vecWI.begin();

    int iVtxIdx = 0;
    for (iter; iter != _pContainer->vecWI.end(); ++iter, ++iVtxIdx)
    {
        if ((*iter).size() > 1)
        {
            // ����ġ �� ������ �������� ����
            sort((*iter).begin(), (*iter).end(),
                 [](const tWeightsAndIndices& left, const tWeightsAndIndices& right) { return left.dWeight > right.dWeight; });

            double dWeight = 0.f;
            for (UINT i = 0; i < (*iter).size(); ++i)
            {
                dWeight += (*iter)[i].dWeight;
            }

            // ����ġ�� ���� 1�� �Ѿ�� ó���κп� �����ش�.
            double revision = 0.f;
            if (dWeight > 1.0)
            {
                revision = 1.0 - dWeight;
                (*iter)[0].dWeight += revision;
            }

            if ((*iter).size() >= 4)
            {
                (*iter).erase((*iter).begin() + 4, (*iter).end());
            }
        }

        // ���� ������ ��ȯ,
        float fWeights[4] = {};
        float fIndices[4] = {};

        for (UINT i = 0; i < (*iter).size(); ++i)
        {
            fWeights[i] = (float)(*iter)[i].dWeight;
            fIndices[i] = (float)(*iter)[i].iBoneIdx;
        }

        memcpy(&_pContainer->vecWeights[iVtxIdx], fWeights, sizeof(Vec4));
        memcpy(&_pContainer->vecIndices[iVtxIdx], fIndices, sizeof(Vec4));
    }
}

void CFBXLoader::LoadKeyframeTransform(FbxNode* _pNode, FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform, int _iBoneIdx,
                                       tContainer* _pContainer)
{
    if (m_vecAnimClip.empty())
        return;

    // ���� ������ ����
    if (!m_vecBone[_iBoneIdx]->vecKeyFrame.empty())
        return;

    FbxVector4 v1 = {1, 0, 0, 0};
    FbxVector4 v2 = {0, 0, 1, 0};
    FbxVector4 v3 = {0, 1, 0, 0};
    FbxVector4 v4 = {0, 0, 0, 1};
    FbxAMatrix matReflect;
    matReflect.mData[0] = v1;
    matReflect.mData[1] = v2;
    matReflect.mData[2] = v3;
    matReflect.mData[3] = v4;

    m_vecBone[_iBoneIdx]->matBone = _matNodeTransform;

    FbxTime::EMode eTimeMode = m_pScene->GetGlobalSettings().GetTimeMode();

    // DirectX ������ ��ȯ�ϴ� ȸ�� ���
    static Matrix OffsetRotmat = XMMatrixRotationX(-XM_PIDIV2) * XMMatrixRotationY(XM_PI);
    static FbxAMatrix OffsetRotFbxMat = GetFbxMatrixFromMatrix(OffsetRotmat);

    for (UINT i = 0; i < m_vecAnimClip.size(); ++i)
    {
        // �ִϸ��̼� ����
        FbxAnimStack* animStack = m_pScene->FindMember<FbxAnimStack>(ToString(m_vecAnimClip[i]->strName).c_str());
        m_pScene->SetCurrentAnimationStack(animStack);

        FbxLongLong llStartFrame = m_vecAnimClip[i]->tStartTime.GetFrameCount(eTimeMode);
        FbxLongLong llEndFrame = m_vecAnimClip[i]->tEndTime.GetFrameCount(eTimeMode);

        for (FbxLongLong frame = llStartFrame; frame <= llEndFrame; ++frame)
        {
            tKeyFrame tFrame = {};
            FbxTime tTime = FbxTime();

            tTime.SetFrame(frame, eTimeMode);

            FbxAMatrix matFromNode = _pNode->EvaluateGlobalTransform(tTime) * _matNodeTransform;
            FbxAMatrix matCurTrans = matFromNode.Inverse() * _pCluster->GetLink()->EvaluateGlobalTransform(tTime);

            matCurTrans = matReflect * matCurTrans * matReflect;

            tFrame.dTime = tTime.GetSecondDouble();
            tFrame.matTransform = OffsetRotFbxMat * matCurTrans;

            m_vecBone[_iBoneIdx]->vecKeyFrame.push_back(tFrame);
        }
    }
}

void CFBXLoader::LoadOffsetMatrix(FbxCluster* _pCluster, const FbxAMatrix& _matNodeTransform, int _iBoneIdx, tContainer* _pContainer)
{
    FbxAMatrix matClusterTrans;
    FbxAMatrix matClusterLinkTrans;

    _pCluster->GetTransformMatrix(matClusterTrans);
    _pCluster->GetTransformLinkMatrix(matClusterLinkTrans);

    // Reflect Matrix
    FbxVector4 V0 = {1, 0, 0, 0};
    FbxVector4 V1 = {0, 0, 1, 0};
    FbxVector4 V2 = {0, 1, 0, 0};
    FbxVector4 V3 = {0, 0, 0, 1};

    FbxAMatrix matReflect;
    matReflect[0] = V0;
    matReflect[1] = V1;
    matReflect[2] = V2;
    matReflect[3] = V3;

    FbxAMatrix matOffset;
    matOffset = matClusterLinkTrans.Inverse() * matClusterTrans * _matNodeTransform;
    matOffset = matReflect * matOffset * matReflect;

    m_vecBone[_iBoneIdx]->matOffset = matOffset;
}

void CFBXLoader::LoadWeightsAndIndices(FbxCluster* _pCluster, int _iBoneIdx, tContainer* _pContainer)
{
    int iIndicesCount = _pCluster->GetControlPointIndicesCount();

    for (int i = 0; i < iIndicesCount; ++i)
    {
        tWeightsAndIndices tWI = {};

        // �� �������� �� �ε��� ������, ����ġ ���� �˸���.
        tWI.iBoneIdx = _iBoneIdx;
        tWI.dWeight = _pCluster->GetControlPointWeights()[i];

        int iVtxIdx = _pCluster->GetControlPointIndices()[i];

        _pContainer->vecWI[iVtxIdx].push_back(tWI);
    }
}

int CFBXLoader::FindBoneIndex(string _strBoneName)
{
    wstring strBoneName = wstring(_strBoneName.begin(), _strBoneName.end());

    for (UINT i = 0; i < m_vecBone.size(); ++i)
    {
        if (m_vecBone[i]->strBoneName == strBoneName)
            return i;
    }

    return -1;
}

FbxAMatrix CFBXLoader::GetTransform(FbxNode* _pNode)
{
    const FbxVector4 vT = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 vR = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 vS = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);

    return FbxAMatrix(vT, vR, vS);
}
