#include "pch.h"
#include "CCamera.h"

#include "CAssetMgr.h"
#include "CEditorMgr.h"
#include "CKeyMgr.h"
#include "CInstancingBuffer.h"

#include "CDevice.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CMeshRender.h"
#include "CMaterial.h"

#include "CRenderComponent.h"
#include "CConstBuffer.h"
#include "CMRT.h"
#include "CLight3D.h"
#include "CAnimator3D.h"

CCamera::CCamera()
    : CComponent(COMPONENT_TYPE::CAMERA)
    , m_Frustum(this)
    , m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
    , m_FOV(XM_PI / 2.f)
    , m_Width(0.f)
    , m_Scale(1.f)
    , m_AspectRatio(1.f)
    , m_Near(1.f)
    , m_Far(10000.f)
    , m_LayerMask(0)
    , m_iCamPriority(-1)
    , m_bHDRI(false)
    , m_matView()
    , m_matViewInv()
    , m_matProj()
    , m_matProjInv()
    , m_mapInstGroup_D{}
    , m_mapInstGroup_F{}
    , m_mapSingleObj{}
    , m_vecDecal{}
    , m_vecTransparent{}
    , m_vecPostProcess{}
    , m_vecShadow{}
{
    Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
    m_Width = vResol.x;
    m_AspectRatio = vResol.x / vResol.y;
}

CCamera::CCamera(const CCamera& origin)
    : CComponent(origin)
    , m_Frustum(this)
    , m_ProjType(origin.m_ProjType)
    , m_FOV(origin.m_FOV)
    , m_Width(origin.m_Width)
    , m_Scale(origin.m_Scale)
    , m_AspectRatio(origin.m_AspectRatio)
    , m_Near(origin.m_Near)
    , m_Far(origin.m_Far)
    , m_LayerMask(origin.m_LayerMask)
    , m_iCamPriority(-1)
    , m_bHDRI(origin.m_bHDRI)
    , m_matView()
    , m_matViewInv()
    , m_matProj()
    , m_matProjInv()
    , m_mapInstGroup_D{}
    , m_mapInstGroup_F{}
    , m_mapSingleObj{}
    , m_vecDecal{}
    , m_vecTransparent{}
    , m_vecPostProcess{}
    , m_vecShadow{}
{
}

CCamera::~CCamera()
{
}

void CCamera::begin()
{
    CRenderMgr::GetInst()->RegisterCamera(this, m_iCamPriority);
}

void CCamera::finaltick()
{
    // =====================
    // 뷰 행렬 계산
    // =====================
    // 카메라를 원점으로 이동시키는 이동 행렬
    Vec3 vCamPos = Transform()->GetWorldPos();
    Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

    //// 카메라의 각 우, 상, 전 방 방향을 기저축이랑 일치시키도록 회전하는 회전행렬
    Vec3 vCamRot = Transform()->GetWorldRotation();
    Matrix matRotate = Matrix::CreateRotationX(vCamRot.x) * Matrix::CreateRotationY(vCamRot.y) * Matrix::CreateRotationZ(vCamRot.z);
    matRotate = matRotate.Transpose(); // 직교행렬의 역행렬은 전치행렬

    // 이동 x 회전 = view 행렬
    m_matView = matTrans * matRotate;
    m_matViewInv = m_matView.Invert();

    // ===============================
    // 투영 방식에 따른 투영 행렬 계산
    // ===============================
    m_matProj = XMMatrixIdentity();

    if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
    {
        // 최소 스케일 제한
        if (m_Scale <= 0.f)
            m_Scale = 1e-5f;

        // 직교투영
        m_matProj = XMMatrixOrthographicLH(m_Width * m_Scale, (m_Width / m_AspectRatio) * m_Scale, m_Near, m_Far);
    }
    else
    {
        // FOV 제한
        float offset = XM_1DIV2PI; // 1/2π 1도 제한
        if (m_FOV < XM_1DIV2PI)
            m_FOV = offset;
        else if (m_FOV > XM_PI - offset)
            m_FOV = XM_PI - offset;

        // 원근투영
        m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_Near, m_Far);
    }

    m_matProjInv = m_matProj.Invert();

    // Frustum 계산
    m_Frustum.finaltick();
}

void CCamera::LayerMask(UINT _LayerIdx, bool _bMask)
{
    if (_bMask)
    {
        m_LayerMask |= (1 << _LayerIdx);
    }
    else
    {
        m_LayerMask &= ~(1 << _LayerIdx);
    }
}

void CCamera::LayerMask(CLevel* _CurLevel, const wstring& _strLayerName, bool _bMask)
{
    CLayer* pLayer = _CurLevel->GetLayer(_strLayerName);

    if (nullptr == pLayer)
        return;

    int idx = pLayer->GetLayerIdx();
    LayerMask(idx, _bMask);
}

void CCamera::SortObject()
{
    // 이전 프레임 분류정보 제거
    render_Clear();

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        // 카메라가 찍도록 설정된 Layer 가 아니면 무시
        if (false == (m_LayerMask & (1 << i)))
            continue;

        CLayer* pLayer = pCurLevel->GetLayer(i);
        const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
        for (size_t j = 0; j < vecObjects.size(); ++j)
        {
            CRenderComponent* pRenderCom = vecObjects[j]->GetRenderComponent();

            // 렌더링 기능이 없는 오브젝트는 제외
            if (nullptr == pRenderCom || nullptr == pRenderCom->GetMesh())
                continue;

            // FrustumCheck
            if (pRenderCom->IsFrustumCheck())
            {
                if (!m_Frustum.FrustumCheckBySphere(vecObjects[j]->Transform()->GetWorldPos(),
                                                    vecObjects[j]->GetRenderComponent()->GetBoundingRadius()))
                    continue;
            }

            // 메테리얼 개수만큼 반복
            UINT iMtrlCount = pRenderCom->GetMtrlCount();
            for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
            {
                // 재질이 없거나, 재질의 쉐이더가 설정이 안된 경우
                if (nullptr == pRenderCom->GetMaterial(iMtrl) || nullptr == pRenderCom->GetMaterial(iMtrl)->GetShader())
                {
                    continue;
                }

                // 쉐이더 도메인에 따른 분류
                SHADER_DOMAIN eDomain = pRenderCom->GetMaterial(iMtrl)->GetShader()->GetDomain();
                Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();

                switch (eDomain)
                {
                case SHADER_DOMAIN::DOMAIN_DEFERRED:
                case SHADER_DOMAIN::DOMAIN_OPAQUE:
                case SHADER_DOMAIN::DOMAIN_MASKED: {
                    // Shader 의 DOMAIN 에 따라서 인스턴싱 그룹을 분류한다.
                    map<ULONG64, vector<tInstObj>>* pMap = NULL;
                    Ptr<CMaterial> pMtrl = pRenderCom->GetMaterial(iMtrl);

                    if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
                    {
                        pMap = &m_mapInstGroup_D;
                    }
                    else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE || pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASKED)
                    {
                        pMap = &m_mapInstGroup_F;
                    }
                    else
                    {
                        assert(nullptr);
                        continue;
                    }

                    uInstID uID = {};
                    uID.llID = pRenderCom->GetInstID(iMtrl);

                    // ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
                    if (0 == uID.llID)
                        continue;

                    map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
                    if (iter == pMap->end())
                    {
                        pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{vecObjects[j], iMtrl}}));
                    }
                    else
                    {
                        iter->second.push_back(tInstObj{vecObjects[j], iMtrl});
                    }
                }
                break;
                case SHADER_DOMAIN::DOMAIN_DECAL:
                    m_vecDecal.push_back(vecObjects[j]);
                    break;
                case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
                    m_vecTransparent.push_back(vecObjects[j]);
                    break;
                case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
                    m_vecPostProcess.push_back(vecObjects[j]);
                    break;
                }
            }
        }
    }
}

void CCamera::SortShadowMapObject(UINT _MobilityType)
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    for (int i = 0; i < LAYER_MAX; ++i)
    {
        // 카메라가 찍도록 설정된 Layer 가 아니면 무시
        if (false == (m_LayerMask & (1 << i)))
            continue;

        CLayer* pLayer = pCurLevel->GetLayer(i);
        const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
        for (size_t j = 0; j < vecObjects.size(); ++j)
        {
            CRenderComponent* pRenderCom = vecObjects[j]->GetRenderComponent();

            // 렌더링 기능이 없는 오브젝트는 제외
            if (nullptr == pRenderCom || nullptr == pRenderCom->GetMesh())
                continue;

            // _MobilityType 가 0 인경우 무조건 추가
            // 그 외의 경우에는 RenderComponent의 옵션 확인
            if (0 == _MobilityType ||
                (vecObjects[j]->GetRenderComponent()->IsCastShadow() && (int)vecObjects[j]->Transform()->GetMobilityType() & _MobilityType))
            {
                m_vecShadow.push_back(vecObjects[j]);
            }
        }
    }
}

void CCamera::render_Deferred()
{
    // 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
    g_Transform.matView = m_matView;
    g_Transform.matViewInv = m_matViewInv;
    g_Transform.matProj = m_matProj;
    g_Transform.matProjInv = m_matProjInv;

    // Deferred Render Pass
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->OMSet();
    render_Deferred_Inst();

    // Decal Pass
    render_Decal();

    // SSAO Pass
    render_SSAO();

    // Light Pass
    render_Light();

    // RenderTarget 설정
    if (m_bHDRI)
    {
        CRenderMgr::GetInst()->GetMRT(MRT_TYPE::HDRI)->OMSet();
    }
    else
    {
        CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
    }

    // Merge Pass
    render_Merge();
}

void CCamera::render_Forward()
{
    // 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
    g_Transform.matView = m_matView;
    g_Transform.matViewInv = m_matViewInv;
    g_Transform.matProj = m_matProj;
    g_Transform.matProjInv = m_matProjInv;

    // RenderTarget 설정
    if (m_bHDRI)
    {
        CRenderMgr::GetInst()->GetMRT(MRT_TYPE::HDRI)->OMSet();
    }
    else
    {
        CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
    }
    //
    // #ifndef DISTRIBUTE
    //    // OutLine Pass
    //    render_OutLine(m_vecDeferred);
    //    render_OutLine(m_vecOpaque);
    //    render_OutLine(m_vecMaked);
    //    render_OutLine(m_vecTransparent);
    // #endif // DISTRIBUTE

    // Main Render Pass
    render_Forward_Inst();
    render(m_vecTransparent);

    // #ifndef DISTRIBUTE
    //     // IDMap Pass
    //     CRenderMgr::GetInst()->GetMRT(MRT_TYPE::IDMAP)->OMSet();
    //     render_IDMap(m_vecDeferred);
    //     render_IDMap(m_vecOpaque);
    //     render_IDMap(m_vecMaked);
    //     render_IDMap(m_vecTransparent);
    // #endif // DISTRIBUTE

    // 후처리
    if (m_bHDRI)
    {
        CRenderMgr::GetInst()->render_postprocess_HDRI();
    }
    else
    {
        CRenderMgr::GetInst()->render_postprocess_LDRI();
    }

    render_Postprocess();

    // Clear
    render_Clear();
}

void CCamera::render_Decal()
{
    CMRT* pDeferredPBRMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED);

    static Ptr<CMaterial> pDecalMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl");

    // Normal Texture Copy
    CONTEXT->CopyResource(pDecalMtrl->GetTexParam(TEX_1)->GetTex2D().Get(), pDeferredPBRMRT->GetRenderTargetTex(2)->GetTex2D().Get());

    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DECAL)->OMSet();

    for (size_t i = 0; i < m_vecDecal.size(); ++i)
    {
        m_vecDecal[i]->render();
    }
}

void CCamera::render_SSAO()
{
    CMRT* pSSAOMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SSAO);
    Ptr<CTexture> pSSAOTex = pSSAOMRT->GetRenderTargetTex(0);

    if (!g_Global.g_EnableSSAO)
    {
        pSSAOTex->UpdateData(30);
        return;
    }

    CTexture::Clear(30);

    pSSAOMRT->OMSet();

    // SSAO
    static Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
    static Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SSAOMtrl");
    pMtrl->UpdateData();
    pMesh->render(0);

    // Blur
    CRenderMgr::GetInst()->BlurTexture(pSSAOTex, 3);

    // Bind
    CONTEXT->OMSetRenderTargets(0, NULL, NULL);
    pSSAOTex->UpdateData(30);
}

void CCamera::render_Light()
{
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::LIGHT)->OMSet();

    const vector<CLight3D*>& vecLight3D = CRenderMgr::GetInst()->GetvecLight3D();
    for (UINT Idx = 0; Idx < vecLight3D.size(); ++Idx)
    {
        vecLight3D[Idx]->render_Deferred(Idx);
    }
}

void CCamera::render_Merge()
{
    static Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
    static Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredMergeMtrl");
    // static Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Merge_DeferredMtrl"); // Phong

    pMtrl->UpdateData();
    pMesh->render(0);
}

void CCamera::render_DepthOnly(Ptr<CTexture> _DepthMapTex)
{
    g_Transform.matView = m_matView;
    g_Transform.matViewInv = m_matViewInv;
    g_Transform.matProj = m_matProj;
    g_Transform.matProjInv = m_matProjInv;

    CONTEXT->ClearDepthStencilView(_DepthMapTex->GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    CONTEXT->OMSetRenderTargets(0, NULL, _DepthMapTex->GetDSV().Get());
    CDevice::GetInst()->SetViewport((float)_DepthMapTex->GetWidth(), (float)_DepthMapTex->GetHeight());

    render_DepthOnly(m_vecShadow);

    m_vecShadow.clear();

    CONTEXT->OMSetRenderTargets(0, NULL, NULL);
}

void CCamera::render_Deferred_Inst()
{
    m_mapSingleObj.clear();

    // Deferred object render
    tInstancingData tInstData = {};

    for (auto& pair : m_mapInstGroup_D)
    {
        // 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
        if (pair.second.empty())
            continue;

        // instancing 개수 조건 미만이거나
        // Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
        // Shader 가 Instancing 을 지원하지 않는경우
        if (pair.second.size() <= INSTANCE_COUNT || pair.second[0].pObj->Animator2D() ||
            nullptr == pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst())
        {
            // 해당 물체들은 단일 랜더링으로 전환
            for (UINT i = 0; i < pair.second.size(); ++i)
            {
                map<INT_PTR, vector<tInstObj>>::iterator iter = m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

                if (iter != m_mapSingleObj.end())
                {
                    iter->second.push_back(pair.second[i]);
                }
                else
                {
                    m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
                }
            }
            continue;
        }

        CGameObject* pObj = pair.second[0].pObj;
        Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
        Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

        // Instancing 버퍼 클리어
        CInstancingBuffer::GetInst()->Clear();

        int iRowIdx = 0;
        bool bHasAnim3D = false;
        for (UINT i = 0; i < pair.second.size(); ++i)
        {
            tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
            tInstData.matWorldInvTranspose = tInstData.matWorld;
            tInstData.matWorldInvTranspose.Translation(Vec3(0.0f));
            tInstData.matWorldInvTranspose = tInstData.matWorldInvTranspose.Transpose().Invert();
            tInstData.matWV = tInstData.matWorld * m_matView;
            tInstData.matWVP = tInstData.matWV * m_matProj;

            if (pair.second[i].pObj->Animator3D())
            {
                pair.second[i].pObj->Animator3D()->UpdateData();
                tInstData.iRowIdx = iRowIdx++;
                CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
                bHasAnim3D = true;
            }
            else
            {
                tInstData.iRowIdx = -1;
            }

            CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
        }

        // 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
        CInstancingBuffer::GetInst()->SetData();

        if (bHasAnim3D)
        {
            pMtrl->SetAnim3D(true); // Animation Mesh 알리기
            pMtrl->SetBoneCount(pMesh->GetBoneCount());
        }

        pMtrl->UpdateData_Inst();
        pMesh->render_instancing(pair.second[0].iMtrlIdx);

        // 정리
        if (bHasAnim3D)
        {
            pMtrl->SetAnim3D(false); // Animation Mesh 알리기
            pMtrl->SetBoneCount(0);
        }
    }

    // 개별 랜더링
    for (auto& pair : m_mapSingleObj)
    {
        if (pair.second.empty())
            continue;

        pair.second[0].pObj->Transform()->UpdateData();

        for (auto& instObj : pair.second)
        {
            instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
        }

        if (pair.second[0].pObj->Animator3D())
        {
            pair.second[0].pObj->Animator3D()->ClearData();
        }
    }
}

void CCamera::render_Forward_Inst()
{
    m_mapSingleObj.clear();

    // Deferred object render
    tInstancingData tInstData = {};

    for (auto& pair : m_mapInstGroup_F)
    {
        // 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
        if (pair.second.empty())
            continue;

        // instancing 개수 조건 미만이거나
        // Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
        // Shader 가 Instancing 을 지원하지 않는경우
        if (pair.second.size() <= INSTANCE_COUNT || pair.second[0].pObj->Animator2D() ||
            nullptr == pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst())
        {
            // 해당 물체들은 단일 랜더링으로 전환
            for (UINT i = 0; i < pair.second.size(); ++i)
            {
                map<INT_PTR, vector<tInstObj>>::iterator iter = m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

                if (iter != m_mapSingleObj.end())
                {
                    iter->second.push_back(pair.second[i]);
                }
                else
                {
                    m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
                }
            }
            continue;
        }

        CGameObject* pObj = pair.second[0].pObj;
        Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
        Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

        // Instancing 버퍼 클리어
        CInstancingBuffer::GetInst()->Clear();

        int iRowIdx = 0;
        bool bHasAnim3D = false;
        for (UINT i = 0; i < pair.second.size(); ++i)
        {
            tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
            tInstData.matWorldInvTranspose = tInstData.matWorld;
            tInstData.matWorldInvTranspose.Translation(Vec3(0.0f));
            tInstData.matWorldInvTranspose = tInstData.matWorldInvTranspose.Transpose().Invert();
            tInstData.matWV = tInstData.matWorld * m_matView;
            tInstData.matWVP = tInstData.matWV * m_matProj;

            if (pair.second[i].pObj->Animator3D())
            {
                pair.second[i].pObj->Animator3D()->UpdateData();
                tInstData.iRowIdx = iRowIdx++;
                CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
                bHasAnim3D = true;
            }
            else
            {
                tInstData.iRowIdx = -1;
            }

            CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
        }

        // 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
        CInstancingBuffer::GetInst()->SetData();

        if (bHasAnim3D)
        {
            pMtrl->SetAnim3D(true); // Animation Mesh 알리기
            pMtrl->SetBoneCount(pMesh->GetBoneCount());
        }

        pMtrl->UpdateData_Inst();
        pMesh->render_instancing(pair.second[0].iMtrlIdx);

        // 정리
        if (bHasAnim3D)
        {
            pMtrl->SetAnim3D(false); // Animation Mesh 알리기
            pMtrl->SetBoneCount(0);
        }
    }

    // 개별 랜더링
    for (auto& pair : m_mapSingleObj)
    {
        if (pair.second.empty())
            continue;

        pair.second[0].pObj->Transform()->UpdateData();

        for (auto& instObj : pair.second)
        {
            instObj.pObj->GetRenderComponent()->render(instObj.iMtrlIdx);
        }

        if (pair.second[0].pObj->Animator3D())
        {
            pair.second[0].pObj->Animator3D()->ClearData();
        }
    }
}

void CCamera::render_Clear()
{
    m_mapInstGroup_D.clear();
    m_mapInstGroup_F.clear();
    m_mapSingleObj.clear();

    m_vecDecal.clear();
    m_vecTransparent.clear();
    m_vecPostProcess.clear();
}

void CCamera::render(vector<CGameObject*>& _vecObj)
{
    for (size_t i = 0; i < _vecObj.size(); ++i)
    {
#ifdef DISTRIBUTE
        _vecObj[i]->render();
#else
        // Render Pass
        if (g_Global.g_DrawAsWireFrame)
        {
            RS_TYPE originRSType = _vecObj[i]->GetRenderComponent()->GetMaterial(0)->GetShader()->GetRSType();
            _vecObj[i]->GetRenderComponent()->GetMaterial(0)->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
            _vecObj[i]->render();
            _vecObj[i]->GetRenderComponent()->GetMaterial(0)->GetShader()->SetRSType(originRSType);
        }
        else
        {
            _vecObj[i]->render();
        }
#endif // DISTRIBUTE
    }
}

void CCamera::render_OutLine(vector<CGameObject*>& _vecObj)
{
    CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();

    for (size_t i = 0; i < _vecObj.size(); ++i)
    {
        if (pSelectedObj != _vecObj[i] || this != CRenderMgr::GetInst()->GetMainCamera() || g_Global.g_DrawAsWireFrame)
            continue;

        // OutLine Pass
        if (g_Global.g_RenderOutline)
        {
            if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
            {
                pSelectedObj->render(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"2D_OutLineMtrl"));
            }
            else
            {
                pSelectedObj->render(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"3D_OutLineMtrl"));
            }
        }

        // NormalLine Pass
        Ptr<CMaterial> NormalLineMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NormalLineMtrl");
        if (NormalLineMtrl->GetMtrlConst().arrInt[0])
        {
            pSelectedObj->render(NormalLineMtrl);
        }
    }
}

void CCamera::render_DepthOnly(vector<CGameObject*>& _vecObj)
{
    for (size_t i = 0; i < _vecObj.size(); i++)
    {
        _vecObj[i]->render(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DepthOnlyMtrl"));
    }
}

void CCamera::render_IDMap(vector<CGameObject*>& _vecObj)
{
    for (size_t i = 0; i < _vecObj.size(); i++)
    {
        wstring LayerName = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(_vecObj[i]->GetLayerIdx())->GetName();
        // 특정 레이어는 IDMap 에서 제외
        if (LayerName != L"UI" && LayerName != L"Camera" && LayerName != L"SkyBox")
        {
            // 오브젝트 이름 + ID값으로 HashID Find
            hash<wstring> hasher;
            int HashID = (int)hasher(_vecObj[i]->GetName()) + _vecObj[i]->GetID();

            Ptr<CMaterial> IDMapMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"IDMapMtrl");
            IDMapMtrl->SetScalarParam(VEC4_0, HashIDToColor(HashID));
            _vecObj[i]->render(IDMapMtrl);
        }
    }
}

void CCamera::render_Postprocess()
{
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

    for (int i = 0; i < m_vecPostProcess.size(); ++i)
    {
        // 최종 렌더링 이미지를 후처리 타겟에 복사
        CRenderMgr::GetInst()->CopyToPostProcessTex_LDRI();

        // 복사받은 후처리 텍스쳐를 t15 레지스터에 바인딩
        CRenderMgr::GetInst()->GetPostProcessTex_LDRI()->UpdateData(15);

        // 후처리 오브젝트 렌더링
        m_vecPostProcess[i]->render();
    }

    CTexture::Clear(15);
}

void CCamera::Resize(Vec2 Resolution)
{
    if (Resolution.x <= 0.f || Resolution.y <= 0.f) // 창 최소화 예외처리
        return;

    m_Width = Resolution.x;
    m_AspectRatio = Resolution.x / Resolution.y;
}

tRay CCamera::GetRay()
{
    // 현재 마우스 좌표
    Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

    float NdcMouseX = 0.0f;
    float NdcMouseY = 0.0f;

    // Mouse Pos → NDC
    if (CEditorMgr::GetInst()->IsEnable())
    {
        vMousePos = CEditorMgr::GetInst()->GetViewportMousePos();
        Vec2 ViewportSize = CEditorMgr::GetInst()->GetViewportSize();

        NdcMouseX = vMousePos.x * 2.0f / ViewportSize.x - 1.0f;
        NdcMouseY = -vMousePos.y * 2.0f / ViewportSize.y + 1.0f;
    }
    else
    {
        Vec2 WindowSize = CDevice::GetInst()->GetRenderResolution();
        NdcMouseX = vMousePos.x * 2.0f / WindowSize.x - 1.0f;
        NdcMouseY = -vMousePos.y * 2.0f / WindowSize.y + 1.0f;
    }

    // NDC → World
    Vector3 cursorNdcNear = Vector3(NdcMouseX, NdcMouseY, 0);
    Vector3 cursorNdcFar = Vector3(NdcMouseX, NdcMouseY, 1);
    Matrix inverseProjView = m_matProjInv * m_matViewInv;
    Vector3 NearWorld = Vector3::Transform(cursorNdcNear, inverseProjView);
    Vector3 FarWorld = Vector3::Transform(cursorNdcFar, inverseProjView);

    // World 좌표 Ray
    tRay ray = {};
    ray.vStart = Transform()->GetWorldPos();
    ray.vDir = (NearWorld - FarWorld).Normalize();

    return ray;
}

void CCamera::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
    fwrite(&m_FOV, sizeof(float), 1, _File);
    fwrite(&m_Width, sizeof(float), 1, _File);
    fwrite(&m_Scale, sizeof(float), 1, _File);
    fwrite(&m_AspectRatio, sizeof(float), 1, _File);
    fwrite(&m_Near, sizeof(float), 1, _File);
    fwrite(&m_Far, sizeof(float), 1, _File);
    fwrite(&m_LayerMask, sizeof(UINT), 1, _File);
    fwrite(&m_iCamPriority, sizeof(int), 1, _File);
    fwrite(&m_bHDRI, sizeof(bool), 1, _File);
}

void CCamera::LoadFromLevelFile(FILE* _File)
{
    fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
    fread(&m_FOV, sizeof(float), 1, _File);
    fread(&m_Width, sizeof(float), 1, _File);
    fread(&m_Scale, sizeof(float), 1, _File);
    fread(&m_AspectRatio, sizeof(float), 1, _File);
    fread(&m_Near, sizeof(float), 1, _File);
    fread(&m_Far, sizeof(float), 1, _File);
    fread(&m_LayerMask, sizeof(UINT), 1, _File);
    fread(&m_iCamPriority, sizeof(int), 1, _File);
    fread(&m_bHDRI, sizeof(bool), 1, _File);
}
