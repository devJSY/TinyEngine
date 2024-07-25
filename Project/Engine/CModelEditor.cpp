#include "pch.h"
#include "CModelEditor.h"

#include "CRenderMgr.h"
#include <Scripts\\CScriptMgr.h>
#include "CEditorMgr.h"

#include "CGameObjectEx.h"

#include "components.h"
#include "CScript.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CMRT.h"
#include "CEngine.h"

CModelEditor::CModelEditor()
    : CEditor(EDITOR_TYPE::MODEL)
    , m_ModelObj(nullptr)
    , m_SelectedBone(nullptr)
    , m_SelectedBoneSocket(nullptr)
    , m_SelectedPreviewObj(nullptr)
    , m_bDrawWireFrame(false)
    , m_bRenderFloor(true)
    , m_bMeshSaved(true)
    , m_bMeshDataSaved(true)
    , m_RecentPath(L"fbx\\")
    , m_MeshDataPath()
    , m_vecDeferred{}
    , m_vecForward{}
    , m_vecTransparent{}
    , m_ViewportRTTex(nullptr)
    , m_ViewportFloatRTTex(nullptr)
    , m_ViewportDSTex(nullptr)
    , m_ViewportCam(nullptr)
    , m_LightObj(nullptr)
    , m_SkyBoxObj(nullptr)
    , m_FloorObj(nullptr)
    , m_ToneMappingObj(nullptr)
    , m_LightBuffer(nullptr)
    , m_ViewportFocused(false)
    , m_ViewportHovered(false)
    , m_GizmoType(ImGuizmo::OPERATION::TRANSLATE)
{
}

CModelEditor::~CModelEditor()
{
    if (nullptr != m_ModelObj)
    {
        delete m_ModelObj;
        m_ModelObj = nullptr;
    }

    if (nullptr != m_ViewportCam)
    {
        delete m_ViewportCam->GetOwner();
        m_ViewportCam = nullptr;
    }

    if (nullptr != m_LightObj)
    {
        delete m_LightObj;
        m_LightObj = nullptr;
    }

    if (nullptr != m_SkyBoxObj)
    {
        delete m_SkyBoxObj;
        m_SkyBoxObj = nullptr;
    }

    if (nullptr != m_FloorObj)
    {
        delete m_FloorObj;
        m_FloorObj = nullptr;
    }

    if (nullptr != m_ToneMappingObj)
    {
        delete m_ToneMappingObj;
        m_ToneMappingObj = nullptr;
    }

    if (nullptr != m_LightBuffer)
    {
        delete m_LightBuffer;
        m_LightBuffer = nullptr;
    }
}

void CModelEditor::init()
{
    // Texture
    Resize(CEngine::GetInst()->GetResolution());

    // Camera
    CGameObjectEx* pCam = new CGameObjectEx;

    pCam->SetName(L"Editor Camera");
    pCam->AddComponent(new CTransform);
    pCam->AddComponent(new CCamera);
    pCam->AddComponent(CScriptMgr::GetScript(MODELEDITORCAMERAMOVESCRIPT));

    pCam->Transform()->SetLocalPos(Vec3(0.f, 250.f, -250.f));
    pCam->Transform()->SetLocalRotation(Vec3(DirectX::XMConvertToRadians(15.f), 0.f, 0.f));

    m_ViewportCam = pCam->Camera();

    m_ViewportCam->LayerMaskAll();
    m_ViewportCam->SetProjType(PROJ_TYPE::PERSPECTIVE);
    m_ViewportCam->SetFOV(XM_PIDIV4);
    m_ViewportCam->SetFar(10000.f);
    m_ViewportCam->SetHDRI(true);

    // Light
    m_LightObj = new CGameObjectEx;
    m_LightObj->SetName(L"Directional Light");
    m_LightObj->AddComponent(new CTransform);
    m_LightObj->AddComponent(new CLight);

    m_LightObj->Transform()->SetLocalPos(Vec3(100.f, 500.f, 100.f));
    m_LightObj->Transform()->SetLocalRotation(Vec3(XMConvertToRadians(45.f), XMConvertToRadians(45.f), 0.f));
    m_LightObj->Light()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
    m_LightObj->Light()->SetLightRadiance(Vec3(0.5f, 0.5f, 0.5f));
    m_LightObj->Light()->SetRadius(5.f);
    m_LightObj->Light()->SetShadowIdx(1);

    // SkyBox
    m_SkyBoxObj = new CGameObjectEx;
    m_SkyBoxObj->SetName(L"SkyBox");
    m_SkyBoxObj->AddComponent(new CTransform);
    m_SkyBoxObj->AddComponent(new CSkyBox);

    m_SkyBoxObj->Transform()->SetLocalPos(Vec3(0.f, 5000.f, 0.f));

    m_SkyBoxObj->SkyBox()->SetEnvTex(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\skybox\\EpicQuadPanorama\\EpicQuadPanoramaEnvHDR.dds",
                                                                          L"Texture\\skybox\\EpicQuadPanorama\\EpicQuadPanoramaEnvHDR.dds"));

    m_SkyBoxObj->SkyBox()->SetBrdfTex(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\skybox\\EpicQuadPanorama\\EpicQuadPanoramaBrdf.dds",
                                                                           L"Texture\\skybox\\EpicQuadPanorama\\EpicQuadPanoramaBrdf.dds"));

    m_SkyBoxObj->SkyBox()->SetDiffuseTex(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\skybox\\EpicQuadPanorama\\EpicQuadPanoramaDiffuseHDR.dds",
                                                                              L"Texture\\skybox\\EpicQuadPanorama\\EpicQuadPanoramaDiffuseHDR.dds"));

    m_SkyBoxObj->SkyBox()->SetSpecularTex(
        CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\skybox\\EpicQuadPanorama\\EpicQuadPanoramaSpecularHDR.dds",
                                             L"Texture\\skybox\\EpicQuadPanorama\\EpicQuadPanoramaSpecularHDR.dds"));

    // Floor
    m_FloorObj = new CGameObjectEx;
    m_FloorObj->SetName(L"Floor");
    m_FloorObj->AddComponent(new CTransform);
    m_FloorObj->AddComponent(new CMeshRender);

    m_FloorObj->Transform()->SetMobilityType(MOBILITY_TYPE::STATIC);
    m_FloorObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
    m_FloorObj->Transform()->SetLocalScale(Vec3(5000.f, 10.f, 5000.f));

    m_FloorObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    m_FloorObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRMtrl"), 0);
    m_FloorObj->MeshRender()->GetDynamicMaterial(0)->SetMaterialCoefficient(Vec4(1.f, 1.f, 1.f, 1.f), Vec4(), Vec4(), 0.f, 0.2f);
    m_FloorObj->MeshRender()->SetFrustumCheck(false);

    // ToneMapping
    m_ToneMappingObj = new CGameObjectEx;
    m_ToneMappingObj->SetName(L"ToneMapping Object");
    m_ToneMappingObj->AddComponent(new CTransform);
    m_ToneMappingObj->AddComponent(new CMeshRender);

    m_ToneMappingObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    m_ToneMappingObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ToneMappingMtrl"), 0);
    m_ToneMappingObj->MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_0, m_ViewportFloatRTTex);
    m_ToneMappingObj->MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_1, nullptr);
    m_ToneMappingObj->MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_2, 0.f); // Bloom Strength
    m_ToneMappingObj->MeshRender()->SetFrustumCheck(false);
    m_ToneMappingObj->MeshRender()->SetCastShadow(false);

    // Light Buffer
    m_LightBuffer = new CStructuredBuffer;
    m_LightBuffer->Create(sizeof(tLightInfo), 1, SB_TYPE::READ_ONLY, true);
}

void CModelEditor::tick()
{
    if (nullptr != m_ModelObj)
    {
        m_ModelObj->tick();
    }

    m_ViewportCam->GetOwner()->tick();
    m_LightObj->Transform()->tick();
    m_LightObj->Light()->GetLightCam()->tick();
    m_SkyBoxObj->tick();
    m_FloorObj->tick();
}

void CModelEditor::finaltick()
{
    if (nullptr != m_ModelObj)
    {
        finaltick_ModelEditor(m_ModelObj);
    }

    m_ViewportCam->GetOwner()->finaltick();
    m_LightObj->Transform()->finaltick();

    // 광원의 카메라도 광원과 동일한 Transform 이 되도록 업데이트
    m_LightObj->Light()->GetLightCam()->Transform()->SetLocalPos(m_LightObj->Transform()->GetWorldPos());
    m_LightObj->Light()->GetLightCam()->Transform()->SetLocalRotation(m_LightObj->Transform()->GetWorldRotation());
    m_LightObj->Light()->GetLightCam()->finaltick();

    m_SkyBoxObj->finaltick();
    m_FloorObj->finaltick();
}

void CModelEditor::render(bool* open)
{
    // =====================================
    // DockSpace
    // =====================================
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetMainViewport()->ID;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplitOther;
    window_class.DockingAllowUnclassed = true;
    ImGui::SetNextWindowClass(&window_class);

    if (!ImGui::Begin(ToString(GetName()).c_str(), open))
    {
        ImGui::End();
        return;
    }

    ImGuiID dockSpace = ImGui::GetID("Model Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // Model Editor Render
    // =====================================
    render();

    ImGui::End();
}

void CModelEditor::render()
{
    ImGui_SetWindowClass(GetEditorType());
    DrawViewport();

    ImGui_SetWindowClass(GetEditorType());
    DrawDetails();

    ImGui_SetWindowClass(GetEditorType());
    DrawSkeletonTree();

    ImGui_SetWindowClass(GetEditorType());
    ImGui::Begin("Animation##ModelEditor");
    DrawAnimation(m_ModelObj, "Model Animation");
    DrawAnimation(m_SelectedPreviewObj, "Preview Model Animation");
    ImGui::End();
}

void CModelEditor::render(const vector<tInstObj>& _vecObj)
{
    for (const tInstObj& pInstObj : _vecObj)
    {
        if (m_bDrawWireFrame)
        {
            Ptr<CMaterial> pMtrl = pInstObj.pObj->GetRenderComponent()->GetMaterial(pInstObj.iMtrlIdx);
            RS_TYPE originRSType = pMtrl->GetShader()->GetRSType();
            pMtrl->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
            pInstObj.pObj->GetRenderComponent()->render(pInstObj.iMtrlIdx);
            pMtrl->GetShader()->SetRSType(originRSType);
        }
        else
        {
            pInstObj.pObj->GetRenderComponent()->render(pInstObj.iMtrlIdx);
        }
    }
}

void CModelEditor::DrawViewport()
{
    ImGui::Begin("Viewport##ModelEditor");

    // 싱테확인
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    // Object Sort
    SortObject();

    // Shadow Map
    render_ShadowMap();

    // =================================
    // 렌더타겟 Clear & 바인딩
    // =================================
    CONTEXT->ClearRenderTargetView(m_ViewportRTTex->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 1.f));
    CONTEXT->ClearRenderTargetView(m_ViewportFloatRTTex->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 1.f));
    CONTEXT->ClearDepthStencilView(m_ViewportDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->Clear();
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::LIGHT)->Clear();

    // 카메라 정보 설정
    g_Transform.matView = m_ViewportCam->GetViewMat();
    g_Transform.matViewInv = m_ViewportCam->GetViewInvMat();
    g_Transform.matProj = m_ViewportCam->GetProjMat();
    g_Transform.matProjInv = m_ViewportCam->GetProjInvMat();

    // 광원 정보 설정
    g_Global.g_LightCount = 1;

    // 메인 카메라 위치 등록
    g_Global.g_eyeWorld = m_ViewportCam->Transform()->GetWorldPos();

    // 전역 상수 데이터 바인딩
    CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
    pGlobalBuffer->SetData(&g_Global);
    pGlobalBuffer->UpdateData();
    pGlobalBuffer->UpdateData_CS();

    // Light 정보 설정
    const tLightInfo& info = m_LightObj->Light()->GetLightInfo();
    tLightInfo CopyInfo = info;

    CCamera* pLightCam = m_LightObj->Light()->GetLightCam()->Camera();

    CopyInfo.vWorldPos = pLightCam->Transform()->GetWorldPos();
    CopyInfo.vWorldDir = pLightCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    CopyInfo.viewMat = pLightCam->GetViewMat();
    CopyInfo.projMat = pLightCam->GetProjMat();
    CopyInfo.invProj = pLightCam->GetProjInvMat();

    m_LightBuffer->SetData(&CopyInfo, 1);
    m_LightBuffer->UpdateData(14);

    // ========================
    // 렌더링
    // ========================
    CONTEXT->OMSetRenderTargets(CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->GetRTCount(),
                                CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->GetRenderTargetView(), m_ViewportDSTex->GetDSV().Get());
    CDevice::GetInst()->SetViewport((float)m_ViewportFloatRTTex->GetWidth(), (float)m_ViewportFloatRTTex->GetHeight());

    m_SkyBoxObj->SkyBox()->UpdateData();

    // Deferred
    render(m_vecDeferred);

    // Light
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::LIGHT)->OMSet();
    m_LightObj->Light()->render_Deferred(0);

    // Merge
    CONTEXT->OMSetRenderTargets(1, m_ViewportFloatRTTex->GetRTV().GetAddressOf(), m_ViewportDSTex->GetDSV().Get());
    CDevice::GetInst()->SetViewport((float)m_ViewportFloatRTTex->GetWidth(), (float)m_ViewportFloatRTTex->GetHeight());

    static Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
    static Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredMergeMtrl");
    pMtrl->UpdateData();
    pMesh->render(0);

    // Forward
    render(m_vecForward);

    // Skybox, Floor
    m_SkyBoxObj->render();
    if (m_bRenderFloor)
    {
        m_FloorObj->render();
    }

    // Transparent
    render(m_vecTransparent);

    // ToneMapping
    CONTEXT->OMSetRenderTargets(1, m_ViewportRTTex->GetRTV().GetAddressOf(), m_ViewportDSTex->GetDSV().Get());
    CDevice::GetInst()->SetViewport((float)m_ViewportRTTex->GetWidth(), (float)m_ViewportRTTex->GetHeight());
    m_ToneMappingObj->render();

    ImGui::Image((void*)m_ViewportRTTex->GetSRV().Get(), ImGui::GetContentRegionAvail());

    // Gizmo
    render_ImGizmo();

    // =========================
    // Clear
    // =========================

    // 렌더타겟 원상복귀
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
    for (UINT i = 0; i < TEX_PARAM::TEX_END; i++)
    {
        CTexture::Clear(i);
    }

    // Skybox Clear
    m_SkyBoxObj->SkyBox()->ClearData();

    // Light Depth Map Clear
    CTexture::Clear(23);

    ImGui::End();
}

void CModelEditor::SortObject()
{
    m_vecDeferred.clear();
    m_vecForward.clear();
    m_vecTransparent.clear();

    if (nullptr == m_ModelObj)
        return;

    list<CGameObject*> queue;
    queue.push_back(m_ModelObj);

    while (!queue.empty())
    {
        CGameObject* pObject = queue.front();
        queue.pop_front();

        // 자식 오브젝트 추가
        vector<CGameObject*>::const_iterator iter = pObject->GetChildObject().begin();
        for (; iter != pObject->GetChildObject().end(); ++iter)
        {
            queue.push_back(*iter);
        }

        // 도메인에 따라 분류
        CRenderComponent* pRenderCom = pObject->GetRenderComponent();

        // 렌더링 기능이 없는 오브젝트는 제외
        if (nullptr == pRenderCom || nullptr == pRenderCom->GetMesh())
            continue;

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
            Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial(iMtrl)->GetShader();
            SHADER_DOMAIN eDomain = pShader->GetDomain();

            switch (eDomain)
            {
            case SHADER_DOMAIN::DOMAIN_SKYBOX:
                break;
            case SHADER_DOMAIN::DOMAIN_DEFERRED:
                m_vecDeferred.push_back(tInstObj{pObject, iMtrl});
                break;
            case SHADER_DOMAIN::DOMAIN_OPAQUE:
            case SHADER_DOMAIN::DOMAIN_MASKED:
                m_vecForward.push_back(tInstObj{pObject, iMtrl});
                break;
            case SHADER_DOMAIN::DOMAIN_DECAL:
                break;
            case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
                m_vecTransparent.push_back(tInstObj{pObject, iMtrl});
                break;
            case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
                break;
            }
        }
    }
}

void CModelEditor::render_ShadowMap()
{
    CCamera* pLightCam = m_LightObj->Light()->GetLightCam()->Camera();

    g_Transform.matView = pLightCam->GetViewMat();
    g_Transform.matViewInv = pLightCam->GetViewInvMat();
    g_Transform.matProj = pLightCam->GetProjMat();
    g_Transform.matProjInv = pLightCam->GetProjInvMat();

    Ptr<CTexture> pLightDepthMapTex = m_LightObj->Light()->GetDepthMapTex();

    CONTEXT->ClearDepthStencilView(pLightDepthMapTex->GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    CONTEXT->OMSetRenderTargets(0, NULL, pLightDepthMapTex->GetDSV().Get());
    CDevice::GetInst()->SetViewport((float)pLightDepthMapTex->GetWidth(), (float)pLightDepthMapTex->GetHeight());

    static Ptr<CMaterial> DepthOnlyMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DepthOnlyMtrl");

    if (nullptr != m_ModelObj)
    {
        list<CGameObject*> queue;
        queue.push_back(m_ModelObj);

        while (!queue.empty())
        {
            CGameObject* pObject = queue.front();
            queue.pop_front();

            pObject->render(DepthOnlyMtrl);

            // 자식 오브젝트 추가
            vector<CGameObject*>::const_iterator iter = pObject->GetChildObject().begin();
            for (; iter != pObject->GetChildObject().end(); ++iter)
            {
                queue.push_back(*iter);
            }
        }
    }

    // ShadowMap Bind
    CONTEXT->OMSetRenderTargets(0, NULL, NULL);
    m_LightObj->Light()->GetDepthMapTex()->UpdateData(23);
}

void CModelEditor::render_ImGizmo()
{
    if (nullptr == m_ModelObj || nullptr == m_ModelObj->Animator() || !m_ModelObj->Animator()->IsValid() || nullptr == m_SelectedBoneSocket)
        return;

    // 선택된 오브젝트가 있을때 키입력으로 Gizmo 타입설정
    if (!KEY_PRESSED(KEY::RBTN))
    {
        if (KEY_TAP(KEY::Q))
            m_GizmoType = (ImGuizmo::OPERATION)0;
        else if (KEY_TAP(KEY::W))
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
        else if (KEY_TAP(KEY::E))
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
        else if (KEY_TAP(KEY::R))
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
    }

    ImGuizmo::SetOrthographic(false);

    ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    Matrix CamView = m_ViewportCam->GetViewMat();
    Matrix CamProj = m_ViewportCam->GetProjMat();

    // Snapping
    bool snap = KEY_PRESSED(KEY::LCTRL);

    float snapValue = 0.f;
    if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
        snapValue = 10.f;
    else if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
        snapValue = 15.f;
    else if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
        snapValue = 1.f;

    float snapValues[3] = {snapValue, snapValue, snapValue};

    Matrix SocketMat = m_SelectedBoneSocket->matSocket;
    Matrix BoneTransformMat = m_ModelObj->Animator()->GetBoneTransformMat(m_SelectedBoneSocket->BoneIndex);
    Matrix WorldMat = m_ModelObj->Transform()->GetWorldMat();

    Matrix mat = SocketMat * BoneTransformMat * WorldMat;

    ImGuizmo::Manipulate(*CamView.m, *CamProj.m, m_GizmoType, ImGuizmo::LOCAL, *mat.m, nullptr, snap ? snapValues : nullptr);

    if (ImGuizmo::IsUsing())
    {
        // Socket Matrix 추출
        mat *= m_ModelObj->Transform()->GetWorldInvMat() * BoneTransformMat.Invert();

        Vec3 SocketTranslation, SocketRotation, SocketScale;
        ImGuizmo::DecomposeMatrixToComponents(*SocketMat.m, SocketTranslation, SocketRotation, SocketScale);

        Vec3 Translation, Rotation, Scale;
        ImGuizmo::DecomposeMatrixToComponents(*mat.m, Translation, Rotation, Scale);

        // 원본 Socket Matrix와 Gizmo Socket Matrix의 변화량 추출
        Vec3 vPosOffset = SocketTranslation - Translation;
        Vec3 vRotOffset = SocketRotation - Rotation;
        vRotOffset.ToRadian();
        Vec3 vScaleOffset = SocketScale - Scale;

        m_SelectedBoneSocket->RelativePosition -= vPosOffset;
        m_SelectedBoneSocket->RelativeRotation -= vRotOffset;
        m_SelectedBoneSocket->RelativeScale -= vScaleOffset;

        m_bMeshSaved = false;
    }
}

void CModelEditor::DrawDetails()
{
    ImGui::Begin("Details##ModelEditor");

    static ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                                                    ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    // ==========================
    // Mesh Data
    // ==========================
    if (ImGui::TreeNodeEx("Mesh Data##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        string ModelName = string();
        if (nullptr != m_ModelObj)
        {
            ModelName = ToString(m_ModelObj->GetName());
        }

        const map<wstring, Ptr<CAsset>>& mapMeshData = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::MESHDATA);

        if (ImGui_ComboUI(ImGui_LabelPrefix("Mesh Data").c_str(), ModelName, mapMeshData))
        {
            SetModel(CAssetMgr::GetInst()->FindAsset<CMeshData>(ToWstring(ModelName)));
        }

        ImGui::Separator();

        if (ImGui_AlignButton("Load Model", 0.f))
        {
            vector<wstring> vec;
            OpenFileDialog(vec, m_RecentPath, {{L"FBX Files", L"*.fbx"}});

            for (wstring& strPath : vec)
            {
                std::filesystem::path filePath = strPath;

                // 취소, 닫기 버튼 체크를 클릭하지 않은 경우
                if (!filePath.empty())
                {
                    // .fbx 포맷이 아닌 경우
                    if (L".fbx" != filePath.extension())
                    {
                        MessageBox(nullptr, L"fbx 포맷 파일이 아닙니다.", L"모델 로딩 실패", MB_ICONHAND);
                    }
                    // 경로에 Content 폴더가 포함되지 않은 경우
                    else if (string::npos == wstring(filePath).find(CPathMgr::GetContentPath()))
                    {
                        m_RecentPath = L"fbx\\";
                        MessageBox(nullptr, L"Content 폴더에 존재하는 모델이 아닙니다.", L"모델 로딩 실패", MB_ICONHAND);
                    }
                    else
                    {
                        m_RecentPath = filePath.lexically_relative(CPathMgr::GetContentPath()).parent_path();
                        CAssetMgr::GetInst()->AsyncLoadFBX(filePath.lexically_relative(CPathMgr::GetContentPath()));
                    }
                }
            }
        }

        if (nullptr != m_ModelObj)
        {
            ImGui::SameLine();

            if (m_bMeshDataSaved)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
            }

            if (ImGui_AlignButton("Save MeshData##ModelEditorDetails", 1.f))
            {
                Ptr<CMeshData> pMeshData = CAssetMgr::GetInst()->FindAsset<CMeshData>(m_MeshDataPath);

                pMeshData->m_pMesh = m_ModelObj->MeshRender()->GetMesh();
                UINT MtrlCount = m_ModelObj->MeshRender()->GetMtrlCount();
                pMeshData->m_vecMtrl.clear();
                pMeshData->m_vecMtrl.resize(MtrlCount);

                for (UINT i = 0; i < MtrlCount; ++i)
                {
                    pMeshData->m_vecMtrl[i] = m_ModelObj->MeshRender()->GetMaterial(i);
                }

                if (S_OK == pMeshData->Save(m_MeshDataPath))
                {
                    MessageBox(nullptr, L"MeshData 저장 성공!", L"Save MeshData", MB_ICONASTERISK);
                    m_bMeshDataSaved = true;
                }
                else
                {
                    MessageBox(nullptr, L"MeshData 저장 실패!", L"Save MeshData", MB_ICONHAND);
                }
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }

        ImGui::TreePop();
    }

    // ==========================
    // Mesh
    // ==========================
    if (ImGui::TreeNodeEx("Mesh##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        if (nullptr != m_ModelObj)
        {
            Ptr<CMesh> pMesh = m_ModelObj->MeshRender()->GetMesh();

            string MeshName;
            if (nullptr != pMesh)
            {
                MeshName = ToString(pMesh->GetName());
            }

            const map<wstring, Ptr<CAsset>>& mapMesh = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::MESH);

            if (ImGui_ComboUI(ImGui_LabelPrefix("Mesh").c_str(), MeshName, mapMesh))
            {
                m_ModelObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(ToWstring(MeshName)));
                m_SelectedBone = nullptr;
                m_SelectedBoneSocket = nullptr;
                m_SelectedPreviewObj = nullptr;
                m_bMeshDataSaved = false;

                // 자식 오브젝트 삭제
                std::stack<CGameObject*> stackChild;

                for (CGameObject* pChild : m_ModelObj->GetChildObject())
                {
                    stackChild.push(pChild);
                }

                while (!stackChild.empty())
                {
                    CGameObject* pChild = stackChild.top();
                    stackChild.pop();

                    pChild->DisconnectWithParent();
                    delete pChild;
                    pChild = nullptr;
                }
            }

            if (nullptr != m_ModelObj && nullptr != m_ModelObj->Animator() && m_ModelObj->Animator()->IsValid())
            {
                ImGui::Separator();
                if (m_bMeshSaved)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
                }

                if (ImGui_AlignButton("Save Mesh##ModelEditorDetails", 0.f))
                {
                    Ptr<CMesh> pMesh = m_ModelObj->Animator()->GetSkeletalMesh();
                    if (S_OK == pMesh->Save(pMesh->GetKey()))
                    {
                        MessageBox(nullptr, L"Mesh 저장 성공!", L"Save Mesh", MB_ICONASTERISK);
                        m_bMeshSaved = true;
                    }
                    else
                    {
                        MessageBox(nullptr, L"Mesh 저장 실패!", L"Save Mesh", MB_ICONHAND);
                    }
                }
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();

                ImGui::SameLine();
                if (ImGui_AlignButton("Import Animation##ModelEditorDetails", 1.f))
                {
                    vector<wstring> vec;
                    OpenFileDialog(vec, m_RecentPath, {{L"FBX Files", L"*.fbx"}});

                    for (wstring& strPath : vec)
                    {
                        std::filesystem::path filePath = strPath;

                        // 취소, 닫기 버튼 체크를 클릭하지 않은 경우
                        if (!filePath.empty())
                        {
                            // .fbx 포맷이 아닌 경우
                            if (L".fbx" != filePath.extension())
                            {
                                MessageBox(nullptr, L"fbx 포맷 파일이 아닙니다.", L"모델 로딩 실패", MB_ICONHAND);
                            }
                            // 경로에 Content 폴더가 포함되지 않은 경우
                            else if (string::npos == wstring(filePath).find(CPathMgr::GetContentPath()))
                            {
                                m_RecentPath = L"fbx\\";
                                MessageBox(nullptr, L"Content 폴더에 존재하는 모델이 아닙니다.", L"모델 로딩 실패", MB_ICONHAND);
                            }
                            else
                            {
                                m_RecentPath = filePath.lexically_relative(CPathMgr::GetContentPath()).parent_path();
                                CAssetMgr::GetInst()->AsyncLoadAnimationFBX(m_ModelObj->Animator()->GetSkeletalMesh(),
                                                                            filePath.lexically_relative(CPathMgr::GetContentPath()));
                            }
                        }
                    }
                }
            }
        }

        ImGui::TreePop();
    }

    // ==========================
    // Material
    // ==========================
    if (ImGui::TreeNodeEx("Material##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        if (nullptr != m_ModelObj)
        {
            CMeshRender* pMeshRender = m_ModelObj->MeshRender();
            Ptr<CMesh> pMesh = pMeshRender->GetMesh();

            if (nullptr != pMesh)
            {
                for (UINT i = 0; i < pMesh->GetSubsetCount(); ++i)
                {
                    Ptr<CMaterial> pCurMtrl = pMeshRender->GetMaterial(i);

                    string CurMtrlname;
                    if (nullptr != pCurMtrl)
                    {
                        CurMtrlname = ToString(pCurMtrl->GetName());
                    }

                    const map<wstring, Ptr<CAsset>>& mapMtrl = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::MATERIAL);

                    if (ImGui_ComboUI(ImGui_LabelPrefix(ToString(pMesh->GetIBName(i)).c_str()).c_str(), CurMtrlname, mapMtrl))
                    {
                        m_ModelObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(ToWstring(CurMtrlname)), i);
                        m_bMeshDataSaved = false;
                    }

                    string MtrlEditorStr = "Material ";
                    MtrlEditorStr += std::to_string(i);
                    MtrlEditorStr += " Editor";
                    if (ImGui_AlignButton(MtrlEditorStr.c_str(), 1.f))
                    {
                        CEditorMgr::GetInst()->GetLevelEditor()->ShowEditor(EDITOR_TYPE::MATERIAL, true);
                        CEditorMgr::GetInst()->GetMaterialEditor()->SetMaterial(pCurMtrl);
                    }
                }
            }
        }

        ImGui::TreePop();
    }

    // ==========================
    // Bone
    // ==========================
    if (nullptr != m_SelectedBone)
    {
        if (ImGui::TreeNodeEx("Bone##ModelEditorDetails", DefaultTreeNodeFlag))
        {
            ImGui_InputText("Bone Name", ToString(m_SelectedBone->strBoneName));

            ImGui::TreePop();
        }

        // ==========================
        // Transforms
        // ==========================
        if (ImGui::TreeNodeEx("Transforms##ModelEditorDetails", DefaultTreeNodeFlag))
        {
            // Bone
            if (ImGui::TreeNodeEx("Bone##ModelEditorDetailsTransforms", DefaultTreeNodeFlag))
            {
                Vec3 pos, rot, scale;
                ImGuizmo::DecomposeMatrixToComponents(*m_ModelObj->Animator()->GetBoneTransformMat(m_SelectedBone->iIdx).m, pos, rot, scale);

                ImGui_DrawVec3Control("Position", pos, 1.f);
                ImGui_DrawVec3Control("Rotation", rot, DirectX::XMConvertToRadians(15.f));
                ImGui_DrawVec3Control("Scale", scale, 1.f, 1.f, D3D11_FLOAT32_MAX, 1.f);

                ImGui::TreePop();
            }

            // Offset
            if (ImGui::TreeNodeEx("Offset##ModelEditorDetailsTransforms", DefaultTreeNodeFlag))
            {
                Vec3 pos, rot, scale;
                ImGuizmo::DecomposeMatrixToComponents(*m_SelectedBone->matOffset.m, pos, rot, scale);

                ImGui_DrawVec3Control("Position", pos, 1.f);
                ImGui_DrawVec3Control("Rotation", rot, DirectX::XMConvertToRadians(15.f));
                ImGui_DrawVec3Control("Scale", scale, 1.f, 1.f, D3D11_FLOAT32_MAX, 1.f);

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }

    // ==========================
    // Bone Socket
    // ==========================
    if (nullptr != m_SelectedBoneSocket)
    {
        if (ImGui::TreeNodeEx("Socket Parameters##ModelEditorDetails", DefaultTreeNodeFlag))
        {
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            string SocketName = ToString(m_SelectedBoneSocket->SoketName);
            strcpy_s(buffer, sizeof(buffer), SocketName.c_str());

            if (ImGui::InputText(ImGui_LabelPrefix("Bone Socket Name").c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                m_SelectedBoneSocket->SoketName = ToWstring(buffer);
            }

            ImGui_InputText("Bone Name",
                            ToString(m_ModelObj->Animator()->GetSkeletalMesh()->GetBones()->at(m_SelectedBoneSocket->BoneIndex).strBoneName).c_str());

            if (ImGui_DrawVec3Control("Relative Position", m_SelectedBoneSocket->RelativePosition, 0.01f, 0.f, 0.f, 0.f, 200.f))
            {
                m_bMeshSaved = false;
            }

            Vec3 rot = m_SelectedBoneSocket->RelativeRotation;
            rot.ToDegree();
            if (ImGui_DrawVec3Control("Relative Rotation", rot, DirectX::XMConvertToRadians(15.f), 0.f, 0.f, 0.f, 200.f))
            {
                m_bMeshSaved = false;
            }
            rot.ToRadian();
            m_SelectedBoneSocket->RelativeRotation = rot;

            if (ImGui_DrawVec3Control("Relative Scale", m_SelectedBoneSocket->RelativeScale, 0.01f, 1.f, D3D11_FLOAT32_MAX, 1.f, 200.f))
            {
                m_bMeshSaved = false;
            }

            ImGui::TreePop();
        }
    }

    // ==========================
    // Options
    // ==========================
    if (ImGui::TreeNodeEx("Options##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        ImGui::Checkbox(ImGui_LabelPrefix("Draw WireFrame").c_str(), &m_bDrawWireFrame);
        ImGui::Checkbox(ImGui_LabelPrefix("Render Floor").c_str(), &m_bRenderFloor);

        ImGui::TreePop();
    }

    ImGui::End();
}

void CModelEditor::DrawSkeletonTree()
{
    ImGui::Begin("Skeleton Tree##ModelEditor");

    if (nullptr != m_ModelObj && nullptr != m_ModelObj->MeshRender()->GetMesh() && m_ModelObj->MeshRender()->GetMesh()->IsSkeletalMesh())
    {
        int NodeOpenFlag = 0;
        if (ImGui::Button("Expand All"))
        {
            NodeOpenFlag = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Collapse All"))
        {
            NodeOpenFlag = -1;
        }

        ImGui::Separator();

        SkeletonRe(*const_cast<vector<tMTBone>*>(m_ModelObj->MeshRender()->GetMesh()->GetBones()), 0, NodeOpenFlag);
    }

    ImGui::End();
}

void CModelEditor::SkeletonRe(vector<tMTBone>& _vecBone, int _BoneIdx, int _NodeOpenFlag)
{
    if (-1 == _NodeOpenFlag)
    {
        ImGui::SetNextItemOpen(false);
    }
    else if (1 == _NodeOpenFlag)
    {
        ImGui::SetNextItemOpen(true);
    }

    static ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

    bool opened = ImGui::TreeNodeEx(ToString(_vecBone[_BoneIdx].strBoneName).c_str(),
                                    m_SelectedBone == &_vecBone[_BoneIdx] ? ImGuiTreeNodeFlags_Selected : 0 | DefaultTreeNodeFlag);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        m_SelectedBone = &_vecBone[_BoneIdx];
        m_SelectedBoneSocket = nullptr;
        m_SelectedPreviewObj = nullptr;
    }

    // Bone PopUp
    string PopUpID = "BonePopUp##ModelEditor";
    PopUpID += std::to_string(_BoneIdx);

    ImGui::OpenPopupOnItemClick(PopUpID.c_str(), ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup(PopUpID.c_str()))
    {
        if (ImGui::MenuItem("Add Socket"))
        {
            wstring SocketName = _vecBone[_BoneIdx].strBoneName;
            tBoneSocket* BoneSocket = new tBoneSocket{SocketName + L"Socket_" + to_wstring(_vecBone[_BoneIdx].vecBoneSocket.size()), _BoneIdx, Vec3(),
                                                      Vec3(), Vec3(1.f, 1.f, 1.f)};

            m_ModelObj->Animator()->GetSkeletalMesh()->AddBoneSocket(_BoneIdx, BoneSocket);
            m_SelectedBoneSocket = BoneSocket;
            m_bMeshSaved = false;
        }

        ImGui::EndPopup();
    }

    if (opened)
    {
        // Bone Socket
        DrawBoneSocket(_vecBone[_BoneIdx]);

        for (UINT i = 0; i < (UINT)_vecBone.size(); ++i)
        {
            // 본인의 BoneIdx과 부모의 BoneIdx가 같은 경우
            if (i == _BoneIdx)
                continue;

            if (_BoneIdx == _vecBone[i].iParentIdx)
            {
                SkeletonRe(_vecBone, i, _NodeOpenFlag);
            }
        }

        ImGui::TreePop();
    }
}

void CModelEditor::DrawBoneSocket(tMTBone& _Bone)
{
    static ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

    for (UINT i = 0; i < (UINT)_Bone.vecBoneSocket.size(); ++i)
    {
        tBoneSocket* pBoneSocket = _Bone.vecBoneSocket[i];

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6667f, 0.6667f, 1.f, 1.f));
        bool bBoneSocketOpend = ImGui::TreeNodeEx(ToString(pBoneSocket->SoketName).c_str(),
                                                  m_SelectedBoneSocket == pBoneSocket ? ImGuiTreeNodeFlags_Selected : 0 | DefaultTreeNodeFlag);
        ImGui::PopStyleColor();

        // Bone Socket Popup
        string BoneSocketPopUpID = "BoneSocketPopUp##ModelEditor";
        BoneSocketPopUpID += ToString(pBoneSocket->SoketName);

        ImGui::OpenPopupOnItemClick(BoneSocketPopUpID.c_str(), ImGuiPopupFlags_MouseButtonRight);

        if (ImGui::BeginPopup(BoneSocketPopUpID.c_str()))
        {
            if (ImGui::MenuItem("Delete Bone Socket"))
            {
                if (m_SelectedBoneSocket == pBoneSocket)
                {
                    m_SelectedBoneSocket = nullptr;
                }

                if (nullptr != m_SelectedPreviewObj && m_SelectedPreviewObj->GetBoneSocket() == pBoneSocket)
                {
                    m_SelectedPreviewObj = nullptr;
                }

                m_ModelObj->Animator()->GetSkeletalMesh()->RemoveBoneSocket(_Bone.iIdx, pBoneSocket);
                --i;

                // BoneSocket에 부착된 오브젝트 삭제
                std::stack<CGameObject*> stackChild;

                for (CGameObject* pChild : m_ModelObj->GetChildObject())
                {
                    if (pBoneSocket == pChild->GetBoneSocket())
                    {
                        stackChild.push(pChild);
                    }
                }

                while (!stackChild.empty())
                {
                    CGameObject* pChild = stackChild.top();
                    stackChild.pop();

                    pChild->DisconnectWithParent();
                    delete pChild;
                    pChild = nullptr;
                }

                // BoneSocket 삭제
                delete pBoneSocket;
                pBoneSocket = nullptr;
                m_bMeshSaved = false;
            }

            if (ImGui::BeginMenu("Add Preview Asset"))
            {
                string PreviewAssetName;
                const map<wstring, Ptr<CAsset>>& mapMeshData = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::MESHDATA);

                if (ImGui_ComboUI("Mesh Data", PreviewAssetName, mapMeshData))
                {
                    Ptr<CMeshData> pMeshData = CAssetMgr::GetInst()->FindAsset<CMeshData>(ToWstring(PreviewAssetName));

                    if (nullptr == pMeshData)
                    {
                        // 해당 소켓 자식 오브젝트 전체 삭제
                        std::stack<CGameObject*> stackChild;

                        for (CGameObject* pChild : m_ModelObj->GetChildObject())
                        {
                            if (pBoneSocket == pChild->GetBoneSocket())
                            {
                                stackChild.push(pChild);
                            }
                        }

                        while (!stackChild.empty())
                        {
                            CGameObject* pChild = stackChild.top();
                            stackChild.pop();

                            pChild->DisconnectWithParent();
                            delete pChild;
                            pChild = nullptr;
                        }
                    }
                    else
                    {
                        // 해당 소켓에 프리뷰 오브젝트 자식오브젝트로 추가
                        CGameObjectEx* PreviewAssetObj = pMeshData->InstantiateEx();

                        PreviewAssetObj->Transform()->SetMobilityType(MOBILITY_TYPE::MOVABLE);
                        PreviewAssetObj->Transform()->SetAbsolute(false);

                        PreviewAssetObj->MeshRender()->SetFrustumCheck(false);

                        PreviewAssetObj->SetBoneSocket(pBoneSocket);
                        m_ModelObj->AddChild(PreviewAssetObj);
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }

        // Bone Socket Select
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            m_SelectedBone = nullptr;
            m_SelectedBoneSocket = pBoneSocket;
            m_SelectedPreviewObj = nullptr;
        }

        if (bBoneSocketOpend)
        {
            // ================
            // Preview Object
            // ================
            CGameObject* pDeleteObject = nullptr;
            for (CGameObject* pChild : m_ModelObj->GetChildObject())
            {
                if (pBoneSocket == pChild->GetBoneSocket())
                {
                    string PreviewStr = ToString(pChild->GetName());
                    PreviewStr += "##ModelEditor";
                    PreviewStr += std::to_string(pChild->GetID());

                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.647f, 0.f, 1.f));
                    bool bPreviewOpend =
                        ImGui::TreeNodeEx(PreviewStr.c_str(), m_SelectedPreviewObj == pChild ? ImGuiTreeNodeFlags_Selected : 0 | DefaultTreeNodeFlag);
                    ImGui::PopStyleColor();

                    // Preview Object Popup
                    string PreviewObjectPopupID = "PreviewObjectPopup";
                    PreviewObjectPopupID += PreviewStr;

                    ImGui::OpenPopupOnItemClick(PreviewObjectPopupID.c_str(), ImGuiPopupFlags_MouseButtonRight);

                    if (ImGui::BeginPopup(PreviewObjectPopupID.c_str()))
                    {
                        if (ImGui::MenuItem("Delete Preview Object"))
                        {
                            pDeleteObject = pChild;
                        }

                        ImGui::EndPopup();
                    }

                    // Preview Object Select
                    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                    {
                        m_SelectedBone = nullptr;
                        m_SelectedBoneSocket = nullptr;
                        m_SelectedPreviewObj = pChild;
                    }

                    if (bPreviewOpend)
                    {
                        ImGui::TreePop();
                    }
                }
            }

            if (nullptr != pDeleteObject)
            {
                if (m_SelectedPreviewObj == pDeleteObject)
                {
                    m_SelectedPreviewObj = nullptr;
                }

                pDeleteObject->DisconnectWithParent();
                delete pDeleteObject;
                pDeleteObject = nullptr;
            }

            ImGui::TreePop();
        }
    }
}

void CModelEditor::DrawAnimation(CGameObject* _Obj, const string& _TreeNodeName)
{
    if (nullptr == _Obj || nullptr == _Obj->Animator())
        return;

    static ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                                                    ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    if (ImGui::TreeNodeEx(_TreeNodeName.c_str(), DefaultTreeNodeFlag))
    {
        CAnimator* pAnimator = _Obj->Animator();
        Ptr<CMesh> pSkeletalMesh = _Obj->Animator()->GetSkeletalMesh();

        // Skeletal Mesh
        if (ImGui::TreeNodeEx("Skeletal Mesh##ModelEditor Animation", DefaultTreeNodeFlag))
        {
            string name;
            if (nullptr != pSkeletalMesh)
                name = ToString(pSkeletalMesh->GetName());

            ImGui_InputText("Skeletal Mesh", name);

            ImGui::TreePop();
        }

        // Animation
        if (nullptr != pSkeletalMesh && pAnimator->IsValid())
        {
            if (ImGui::TreeNodeEx("Animation##ModelEditor Animation", DefaultTreeNodeFlag))
            {
                // =====================
                // Animations
                // =====================
                const vector<tMTAnimClip>* vecAnimClip = pSkeletalMesh->GetAnimClip();

                int CurClipIdx = pAnimator->GetCurClipIdx();
                const tMTAnimClip& CurClip = vecAnimClip->at(CurClipIdx);

                string CurClipName = ToString(CurClip.strAnimName);

                static ImGuiTextFilter filter;
                int ChangedClipIdx = -1;
                ImGui::Text("Animations");
                if (ImGui::BeginCombo("##Anim", CurClipName.c_str()))
                {
                    filter.Draw(ImGui_LabelPrefix("Filter").c_str());
                    ImGui::Separator();

                    for (int i = 0; i < vecAnimClip->size(); i++)
                    {
                        string ClipName = ToString(vecAnimClip->at(i).strAnimName);
                        bool is_selected = (CurClipName == ClipName);

                        if (!filter.PassFilter(ClipName.c_str()))
                            continue;

                        if (ImGui::Selectable(ClipName.c_str(), is_selected))
                        {
                            CurClipName = ClipName;
                            ChangedClipIdx = i;
                        }

                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                if (ChangedClipIdx >= 0)
                {
                    pAnimator->SetCurClipIdx(ChangedClipIdx);
                }

                // Frame Index
                int ClipFrameIdx = pAnimator->GetClipFrameIndex();
                if (ImGui::SliderInt(ImGui_LabelPrefix("Frame Index").c_str(), &ClipFrameIdx, 0, CurClip.iFrameLength - 1))
                {
                    pAnimator->SetClipFrameIndex(ClipFrameIdx);
                }

                bool bPlaying = pAnimator->IsPlaying();
                if (ImGui::Checkbox(ImGui_LabelPrefix("Play").c_str(), &bPlaying))
                    pAnimator->SetPlay(bPlaying);

                bool bRepeat = pAnimator->IsRepeat();
                if (ImGui::Checkbox(ImGui_LabelPrefix("Repeat").c_str(), &bRepeat))
                    pAnimator->SetRepeat(bRepeat);

                bool bReverse = pAnimator->IsReverse();
                if (ImGui::Checkbox(ImGui_LabelPrefix("Reverse").c_str(), &bReverse))
                    pAnimator->SetReverse(bReverse);

                float PlaySpeed = pAnimator->GetPlaySpeed();
                if (ImGui::DragFloat(ImGui_LabelPrefix("Play Speed").c_str(), &PlaySpeed, 0.01f, 0.f, 100.f))
                    pAnimator->SetPlaySpeed(PlaySpeed);

                // =====================
                // Animation Clip Info
                // =====================
                ImGui::Text("Clip Infomation");
                static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                               ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_SizingFixedSame;
                if (ImGui::BeginTable("##AnimClip", 3, flags, ImVec2(600.f, 150.f)))
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Animation Name");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", ToString(CurClip.strAnimName).c_str());

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Clip Index");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", CurClipIdx);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Frame Rate");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", (int)pAnimator->GetFrameRate());

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Frame Range");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", CurClip.iStartFrame);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%d", CurClip.iEndFrame);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Frame Length");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", CurClip.iFrameLength);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Time Range");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%.3f", CurClip.dStartTime);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%.3f", CurClip.dEndTime);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Time Length");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%.3f", CurClip.dTimeLength);

                    ImGui::EndTable();
                }

                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }
}

void CModelEditor::finaltick_ModelEditor(CGameObject* _Obj)
{
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        CComponent* pComp = _Obj->GetComponent((COMPONENT_TYPE)i);
        if (nullptr == pComp)
            continue;

        if (i == (UINT)COMPONENT_TYPE::ANIMATOR)
        {
            _Obj->Animator()->finaltick_ModelEditor(); // Animator finaltick 예외처리
        }
        else
        {
            pComp->finaltick();
        }

        vector<CGameObject*>::const_iterator iter = _Obj->GetChildObject().begin();
        for (; iter != _Obj->GetChildObject().end(); ++iter)
        {
            finaltick_ModelEditor(*iter);
        }
    }
}

void CModelEditor::Resize(Vec2 resolution)
{
    if (nullptr != m_ViewportRTTex)
    {
        CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"ModelEditorViewportRTTex");
        m_ViewportRTTex = nullptr;
    }

    if (nullptr != m_ViewportFloatRTTex)
    {
        CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"ModelEditorViewportFloatRTTex");
        m_ViewportFloatRTTex = nullptr;
    }

    if (nullptr != m_ViewportDSTex)
    {
        CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"ModelEditorViewportDSTex");
        m_ViewportDSTex = nullptr;
    }

    m_ViewportRTTex =
        CAssetMgr::GetInst()->CreateTexture(L"ModelEditorViewportRTTex", (UINT)resolution.x, (UINT)resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                            D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

    m_ViewportFloatRTTex =
        CAssetMgr::GetInst()->CreateTexture(L"ModelEditorViewportFloatRTTex", (UINT)resolution.x, (UINT)resolution.y, DXGI_FORMAT_R16G16B16A16_FLOAT,
                                            D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

    m_ViewportDSTex = CAssetMgr::GetInst()->CreateTexture(L"ModelEditorViewportDSTex", (UINT)resolution.x, (UINT)resolution.y,
                                                          DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);

    if (nullptr != m_ToneMappingObj)
    {
        m_ToneMappingObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, m_ViewportFloatRTTex);
    }
}

void CModelEditor::SetModel(Ptr<CMeshData> _MeshData)
{
    if (nullptr != m_ModelObj)
    {
        delete m_ModelObj;
        m_ModelObj = nullptr;
    }

    // Bone 데이터 초기화
    m_SelectedBone = nullptr;
    m_SelectedBoneSocket = nullptr;
    m_SelectedPreviewObj = nullptr;
    m_bMeshSaved = true;
    m_bMeshDataSaved = true;

    if (nullptr == _MeshData)
    {
        m_MeshDataPath.clear();
        return;
    }

    m_MeshDataPath = _MeshData->GetKey();
    m_ModelObj = _MeshData->InstantiateEx();

    m_ModelObj->Transform()->SetMobilityType(MOBILITY_TYPE::MOVABLE);
    m_ModelObj->Transform()->SetLocalPos(Vec3(0.f, 150.f, 0.f));
    m_ModelObj->Transform()->SetLocalRotation(Vec3(0.f, XM_PI, 0.f));
    m_ModelObj->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));

    m_ModelObj->MeshRender()->SetFrustumCheck(false);

    if (nullptr != m_ModelObj->Animator())
    {
        m_ModelObj->Animator()->SetPlay(false);
    }

    m_bDrawWireFrame = false;
}

void CModelEditor::NotifiedAnimationLoaded()
{
    if (nullptr == m_ModelObj || nullptr == m_ModelObj->Animator() || !m_ModelObj->Animator()->IsValid())
        return;

    m_ModelObj->Animator()->SetSkeletalMesh(m_ModelObj->Animator()->GetSkeletalMesh());
    m_bMeshSaved = false;
}
