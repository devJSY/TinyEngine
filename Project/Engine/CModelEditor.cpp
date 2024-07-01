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

CModelEditor::CModelEditor()
    : CEditor(EDITOR_TYPE::MODEL)
    , m_ModelObj(nullptr)
    , m_SelectedBoneIdx(-1)
    , m_bDrawWireFrame(false)
    , m_ViewportRTTex(nullptr)
    , m_ViewportFloatRTTex(nullptr)
    , m_ViewportDSTex(nullptr)
    , m_ViewportCam(nullptr)
    , m_LightObj(nullptr)
    , m_SkyBoxObj(nullptr)
    , m_FloorObj(nullptr)
    , m_ToneMappingObj(nullptr)
    , m_LightBuffer(nullptr)
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
    m_ViewportRTTex = CAssetMgr::GetInst()->CreateTexture(L"ModelEditorViewportRTTex", 1280, 1280, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                          D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
    m_ViewportFloatRTTex = CAssetMgr::GetInst()->CreateTexture(L"ModelEditorViewportFloatRTTex", 1280, 1280, DXGI_FORMAT_R16G16B16A16_FLOAT,
                                                               D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
    m_ViewportDSTex = CAssetMgr::GetInst()->CreateTexture(L"ModelEditorViewportDSTex", 1280, 1280, DXGI_FORMAT_D24_UNORM_S8_UINT,
                                                          D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);

    // Camera
    CGameObjectEx* pCam = new CGameObjectEx;

    pCam->SetName(L"Editor Camera");
    pCam->AddComponent(new CTransform);
    pCam->AddComponent(new CCamera);
    pCam->AddComponent(CScriptMgr::GetScript(MODELEDITORCAMERAMOVESCRIPT));

    pCam->Transform()->SetRelativePos(Vec3(0.f, 250.f, -250.f));
    pCam->Transform()->SetRelativeRotation(Vec3(DirectX::XMConvertToRadians(15.f), 0.f, 0.f));

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

    m_LightObj->Transform()->SetRelativePos(Vec3(-1000.f, 2500.f, -1000.f));
    m_LightObj->Transform()->SetDirection(Vec3(1.f, -1.f, 1.f));
    m_LightObj->Light()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
    m_LightObj->Light()->SetLightRadiance(Vec3(0.5f, 0.5f, 0.5f));
    m_LightObj->Light()->SetRadius(10.f);
    m_LightObj->Light()->SetShadowIdx(1);

    // SkyBox
    m_SkyBoxObj = new CGameObjectEx;
    m_SkyBoxObj->SetName(L"SkyBox");
    m_SkyBoxObj->AddComponent(new CTransform);
    m_SkyBoxObj->AddComponent(new CSkyBox);

    m_SkyBoxObj->Transform()->SetRelativePos(Vec3(0.f, 5000.f, 0.f));

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
    m_FloorObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    m_FloorObj->Transform()->SetRelativeScale(Vec3(5000.f, 10.f, 5000.f));

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
        for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
        {
            CComponent* pComp = m_ModelObj->GetComponent((COMPONENT_TYPE)i);
            if (nullptr == pComp)
                continue;

            if (i == (UINT)COMPONENT_TYPE::ANIMATOR)
            {
                m_ModelObj->Animator()->finaltick_ModelEditor(); // Animator finaltick 예외처리
            }
            else
            {
                pComp->finaltick();
            }
        }
    }

    m_ViewportCam->GetOwner()->finaltick();
    m_LightObj->Transform()->finaltick();

    // 광원의 카메라도 광원과 동일한 Transform 이 되도록 업데이트
    m_LightObj->Light()->GetLightCam()->Transform()->SetRelativePos(m_LightObj->Transform()->GetWorldPos());
    m_LightObj->Light()->GetLightCam()->Transform()->SetDirection(m_LightObj->Transform()->GetWorldDir(DIR_TYPE::FRONT));
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
    DrawAnimation();
}

void CModelEditor::DrawViewport()
{
    ImGui::Begin("Viewport##ModelEditor");

    // =================================
    // Shadow Map
    // =================================
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
        m_ModelObj->render(DepthOnlyMtrl);
    }

    CONTEXT->OMSetRenderTargets(0, NULL, NULL);

    // ShadowMap Bind
    m_LightObj->Light()->GetDepthMapTex()->UpdateData(23);

    // =================================
    // Render
    // =================================

    // 렌더타겟 설정
    CONTEXT->ClearRenderTargetView(m_ViewportRTTex->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 1.f));
    CONTEXT->ClearRenderTargetView(m_ViewportFloatRTTex->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 1.f));
    CONTEXT->ClearDepthStencilView(m_ViewportDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    CONTEXT->OMSetRenderTargets(1, m_ViewportFloatRTTex->GetRTV().GetAddressOf(), m_ViewportDSTex->GetDSV().Get());
    CDevice::GetInst()->SetViewport(1280.f, 1280.f);

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

    CopyInfo.vWorldPos = pLightCam->Transform()->GetWorldPos();
    CopyInfo.vWorldDir = pLightCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    CopyInfo.viewMat = pLightCam->GetViewMat();
    CopyInfo.projMat = pLightCam->GetProjMat();
    CopyInfo.invProj = pLightCam->GetProjInvMat();

    m_LightBuffer->SetData(&CopyInfo, 1);
    m_LightBuffer->UpdateData(14);

    // 렌더링
    m_SkyBoxObj->render();
    m_FloorObj->render();
    if (nullptr != m_ModelObj)
    {
        if (m_bDrawWireFrame)
        {
            if (nullptr != m_ModelObj->MeshRender()->GetMesh())
            {
                UINT SubsetCnt = m_ModelObj->MeshRender()->GetMesh()->GetSubsetCount();
                for (UINT i = 0; i < SubsetCnt; ++i)
                {
                    Ptr<CMaterial> pMtrl = m_ModelObj->MeshRender()->GetMaterial(i);
                    if (nullptr == pMtrl)
                        continue;

                    RS_TYPE originRSType = pMtrl->GetShader()->GetRSType();
                    pMtrl->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
                    m_ModelObj->MeshRender()->render(i);
                    pMtrl->GetShader()->SetRSType(originRSType);
                }
            }
        }
        else
        {
            m_ModelObj->render();
        }
    }

    // ToneMapping
    CONTEXT->OMSetRenderTargets(1, m_ViewportRTTex->GetRTV().GetAddressOf(), m_ViewportDSTex->GetDSV().Get());
    m_ToneMappingObj->render();

    ImGui::Image((void*)m_ViewportRTTex->GetSRV().Get(), ImGui::GetContentRegionAvail());

    // 렌더타겟 원상복귀
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
    for (UINT i = 0; i < TEX_PARAM::TEX_END; i++)
    {
        CTexture::Clear(i);
    }

    // SkyBox Clear
    CTexture::Clear(17);
    CTexture::Clear(18);
    CTexture::Clear(19);

    // Light Depth Map Clear
    CTexture::Clear(23);

    ImGui::End();
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

        if (ImGui_AlignButton("Load Model", 1.f))
        {
            std::filesystem::path filePath = OpenFileDialog(L"fbx\\", TEXT("FBX Files\0*.fbx\0모든 파일(*.*)\0*.*\0"));

            // 취소, 닫기 버튼 체크를 클릭하지 않은 경우
            if (!filePath.empty())
            {
                // .fbx 포맷이 아닌 경우
                if (L".fbx" != filePath.extension())
                {
                    MessageBox(nullptr, L"fbx 포맷 파일이 아닙니다.", L"모델 로딩 실패", MB_OK);
                }
                // 경로에 Content 폴더가 포함되지 않은 경우
                else if (string::npos == wstring(filePath).find(CPathMgr::GetContentPath()))
                {
                    MessageBox(nullptr, L"Content 폴더에 존재하는 모델이 아닙니다.", L"모델 로딩 실패", MB_OK);
                }
                else
                {
                    // SetModel(CAssetMgr::GetInst()->AsyncLoadFBX(filePath.lexically_relative(CPathMgr::GetContentPath())));
                    CAssetMgr::GetInst()->AsyncLoadFBX(filePath.lexically_relative(CPathMgr::GetContentPath()));
                }
            }
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
    if (ImGui::TreeNodeEx("Bone##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        if (nullptr != m_ModelObj && -1 < m_SelectedBoneIdx && nullptr != m_ModelObj->MeshRender()->GetMesh() &&
            m_ModelObj->MeshRender()->GetMesh()->IsSkeletalMesh())
        {
            const tMTBone& CurBone = m_ModelObj->MeshRender()->GetMesh()->GetBones()->at(m_SelectedBoneIdx);
            ImGui_InputText("Bone Name", ToString(CurBone.strBoneName).c_str());
        }

        ImGui::TreePop();
    }

    // ==========================
    // Transforms
    // ==========================
    if (ImGui::TreeNodeEx("Transforms##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        if (nullptr != m_ModelObj && -1 < m_SelectedBoneIdx && nullptr != m_ModelObj->MeshRender()->GetMesh() &&
            m_ModelObj->MeshRender()->GetMesh()->IsSkeletalMesh())
        {
            const tMTBone& CurBone = m_ModelObj->MeshRender()->GetMesh()->GetBones()->at(m_SelectedBoneIdx);

            // Bone
            if (ImGui::TreeNodeEx("Bone##ModelEditorDetailsTransforms", DefaultTreeNodeFlag))
            {
                Vec3 pos, rot, scale;
                ImGuizmo::DecomposeMatrixToComponents(*m_ModelObj->Animator()->GetFinalBoneMat().at(m_SelectedBoneIdx).m, pos, rot, scale);

                ImGui_DrawVec3Control("Location", pos, 10.f);
                ImGui_DrawVec3Control("Rotation", rot, 1.f);
                ImGui_DrawVec3Control("Scale", scale, 1.f, 1.f, D3D11_FLOAT32_MAX, 1.f);

                ImGui::TreePop();
            }

            // Offset
            if (ImGui::TreeNodeEx("Offset##ModelEditorDetailsTransforms", DefaultTreeNodeFlag))
            {
                const tMTBone& CurBone = m_ModelObj->MeshRender()->GetMesh()->GetBones()->at(m_SelectedBoneIdx);

                Vec3 pos, rot, scale;
                ImGuizmo::DecomposeMatrixToComponents(*CurBone.matOffset.m, pos, rot, scale);

                ImGui_DrawVec3Control("Location", pos, 10.f);
                ImGui_DrawVec3Control("Rotation", rot, 1.f);
                ImGui_DrawVec3Control("Scale", scale, 1.f, 1.f, D3D11_FLOAT32_MAX, 1.f);

                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }

    // ==========================
    // Options
    // ==========================
    if (ImGui::TreeNodeEx("Options##ModelEditorDetails", DefaultTreeNodeFlag))
    {
        ImGui::Checkbox(ImGui_LabelPrefix("Draw WireFrame").c_str(), &m_bDrawWireFrame);

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

        SkeletonRe(*m_ModelObj->MeshRender()->GetMesh()->GetBones(), 0, NodeOpenFlag);
    }

    ImGui::End();
}

void CModelEditor::SkeletonRe(const vector<tMTBone>& _vecBone, int _BoneIdx, int _NodeOpenFlag)
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
                                    m_SelectedBoneIdx == _BoneIdx ? ImGuiTreeNodeFlags_Selected : 0 | DefaultTreeNodeFlag);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        m_SelectedBoneIdx = _BoneIdx;
    }

    if (opened)
    {
        for (UINT i = 0; i < (UINT)_vecBone.size(); ++i)
        {
            // 본인의 BoneIdx과 부모의 BoneIdx가 같은 경우
            if (i == _BoneIdx)
                continue;

            if (_BoneIdx == _vecBone[i].iParentIndx)
            {
                SkeletonRe(_vecBone, i, _NodeOpenFlag);
            }
        }

        ImGui::TreePop();
    }
}

void CModelEditor::DrawAnimation()
{
    ImGui::Begin("Animation##ModelEditor");
    if (nullptr != m_ModelObj && m_ModelObj->Animator())
    {
        static ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                                        ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
                                                        ImGuiTreeNodeFlags_FramePadding;

        CAnimator* pAnimator = m_ModelObj->Animator();
        Ptr<CMesh> pSkeletalMesh = m_ModelObj->Animator()->GetSkeletalMesh();

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
        if (nullptr != pSkeletalMesh && pAnimator->InValid())
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
                if (ImGui::SliderInt(ImGui_LabelPrefix("Frame Index").c_str(), &ClipFrameIdx, 0, CurClip.iFrameLength))
                {
                    pAnimator->SetClipFrameIndex(ClipFrameIdx);
                }

                bool bPlaying = pAnimator->IsPlaying();
                if (ImGui::Checkbox(ImGui_LabelPrefix("Play").c_str(), &bPlaying))
                    pAnimator->SetPlay(bPlaying);

                bool bRepeat = pAnimator->IsRepeat();
                if (ImGui::Checkbox(ImGui_LabelPrefix("Repeat").c_str(), &bRepeat))
                    pAnimator->SetRepeat(bRepeat);

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
    }
    ImGui::End();
}

void CModelEditor::SetModel(Ptr<CMeshData> _MeshData)
{
    if (nullptr != m_ModelObj)
    {
        delete m_ModelObj;
        m_ModelObj = nullptr;
    }

    // Bone 데이터 초기화
    m_SelectedBoneIdx = -1;

    if (nullptr == _MeshData)
    {
        return;
    }

    m_ModelObj = _MeshData->InstantiateEx();

    m_ModelObj->Transform()->SetMobilityType(MOBILITY_TYPE::MOVABLE);
    m_ModelObj->Transform()->SetRelativePos(Vec3(0.f, 150.f, 0.f));
    m_ModelObj->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
    m_ModelObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

    m_ModelObj->MeshRender()->SetFrustumCheck(false);

    // Forward PBR Shader 로 설정
    Ptr<CGraphicsShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"UnrealPBRShader");
    UINT SubsetCnt = m_ModelObj->MeshRender()->GetMesh()->GetSubsetCount();
    for (UINT i = 0; i < SubsetCnt; ++i)
    {
        m_ModelObj->MeshRender()->GetMaterial(i)->SetShader(pShader);
    }

    m_ModelObj->Animator()->SetPlay(false);

    m_bDrawWireFrame = false;
}
