#include "pch.h"
#include "CMaterialEditor.h"
#include "CEditorMgr.h"
#include "CLevelMgr.h"
#include "CKeyMgr.h"
#include "CRenderMgr.h"

#include "CLevel.h"
#include "components.h"
#include "CGameObjectEx.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CMRT.h"

CMaterialEditor::CMaterialEditor()
    : CEditor(EDITOR_TYPE::MATERIAL)
    , m_Mtrl()
    , m_ViewportRTTex(nullptr)
    , m_ViewportCam(nullptr)
    , m_ViewportObj(nullptr)
    , m_LightObj(nullptr)
    , m_LightBuffer(nullptr)
{
}

CMaterialEditor::~CMaterialEditor()
{
    if (nullptr != m_ViewportCam)
    {
        delete m_ViewportCam->GetOwner();
        m_ViewportCam = nullptr;
    }

    if (nullptr != m_ViewportObj)
    {
        delete m_ViewportObj;
        m_ViewportObj = nullptr;
    }

    if (nullptr != m_LightObj)
    {
        delete m_LightObj;
        m_LightObj = nullptr;
    }

    if (nullptr != m_LightBuffer)
    {
        delete m_LightBuffer;
        m_LightBuffer = nullptr;
    }
}

void CMaterialEditor::init()
{
    // RenderTarget
    m_ViewportRTTex = CAssetMgr::GetInst()->CreateTexture(L"MtrlEditorViewportTex", 1280, 1280, DXGI_FORMAT_R16G16B16A16_FLOAT,
                                                          D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

    // Camera
    CGameObjectEx* pCam = new CGameObjectEx;

    pCam->SetName(L"Editor Camera");
    pCam->AddComponent(new CTransform);
    pCam->AddComponent(new CCamera);

    m_ViewportCam = pCam->Camera();

    m_ViewportCam->LayerMaskAll();
    m_ViewportCam->SetProjType(PROJ_TYPE::PERSPECTIVE);
    m_ViewportCam->SetFOV(XM_PI / 2.f);
    m_ViewportCam->SetFar(10000.f);
    m_ViewportCam->SetHDRI(true);
    m_ViewportCam->SetCameraPriority(-100);

    // Object
    m_ViewportObj = new CGameObjectEx;
    m_ViewportObj->SetName(L"Object");
    m_ViewportObj->AddComponent(new CTransform);
    m_ViewportObj->AddComponent(new CMeshRender);

    m_ViewportObj->MeshRender()->SetFrustumCheck(false);
    m_ViewportObj->MeshRender()->SetCastShadow(false);

    m_ViewportObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 500.f));
    m_ViewportObj->Transform()->SetLocalScale(Vec3(250.f, 250.f, 250.f));
    m_ViewportObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));

    // Light
    m_LightObj = new CGameObjectEx;
    m_LightObj->SetName(L"Point Light");
    m_LightObj->AddComponent(new CTransform);
    m_LightObj->AddComponent(new CLight);

    m_LightObj->Transform()->SetLocalPos(Vec3(250.f, 500.f, 0.f));
    m_LightObj->Light()->SetLightType(LIGHT_TYPE::POINT);
    m_LightObj->Light()->SetLightRadiance(Vec3(2.f, 2.f, 2.f));
    m_LightObj->Light()->SetRadius(10.f);
    m_LightObj->Light()->GetLightCam()->Camera()->SetCameraPriority(-100);

    m_LightBuffer = new CStructuredBuffer;
    m_LightBuffer->Create(sizeof(tLightInfo), 1, SB_TYPE::READ_ONLY, true);
}

void CMaterialEditor::render()
{
    if (nullptr == m_Mtrl.Get())
        return;

    ImGui_SetWindowClass(GetEditorType());
    DrawViewport();

    ImGui_SetWindowClass(GetEditorType());
    DrawDetails();
}

void CMaterialEditor::DrawViewport()
{
    ImGui::Begin("Viewport##MaterialEditor");

    // Object Drag Rotation
    if (ImGui::IsWindowHovered() && KEY_PRESSED(KEY::LBTN))
    {
        Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
        Vec3 vRot = m_ViewportObj->Transform()->GetLocalRotation();
        vRot.y -= vDrag.x * XM_PI / 720.f;
        vRot.x -= vDrag.y * XM_PI / 720.f;
        m_ViewportObj->Transform()->SetLocalRotation(vRot);
    }

    // ����Ÿ�� ����
    CONTEXT->ClearRenderTargetView(m_ViewportRTTex->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 1.f));
    CONTEXT->OMSetRenderTargets(1, m_ViewportRTTex->GetRTV().GetAddressOf(), NULL);
    CDevice::GetInst()->SetViewport(1280.f, 1280.f);

    // ī�޶� ���� ����
    g_Transform.matView = m_ViewportCam->GetViewMat();
    g_Transform.matViewInv = m_ViewportCam->GetViewInvMat();
    g_Transform.matProj = m_ViewportCam->GetProjMat();
    g_Transform.matProjInv = m_ViewportCam->GetProjInvMat();

    // ���� ���� ����
    g_Global.g_LightCount = 1;

    // ���� ī�޶� ��ġ ���
    g_Global.g_eyeWorld = m_ViewportCam->Transform()->GetWorldPos();

    // ���� ��� ������ ���ε�
    CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
    pGlobalBuffer->SetData(&g_Global);
    pGlobalBuffer->UpdateData();
    pGlobalBuffer->UpdateData_CS();

    // Light ���� ����
    const tLightInfo& info = m_LightObj->Light()->GetLightInfo();

    tLightInfo CopyInfo = {};
    CopyInfo = info;
    CopyInfo.vWorldPos = m_LightObj->Transform()->GetWorldPos();
    CopyInfo.vWorldDir = m_LightObj->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    CopyInfo.viewMat = Matrix();
    CopyInfo.projMat = Matrix();
    CopyInfo.invProj = Matrix();

    m_LightBuffer->SetData(&CopyInfo, 1);
    m_LightBuffer->UpdateData(14);

    // ������
    if (nullptr != m_Mtrl->GetShader())
    {
        m_ViewportObj->MeshRender()->SetMaterial(m_Mtrl, 0);
        m_ViewportObj->render();
    }

    ImGui::Image((void*)m_ViewportRTTex->GetSRV().Get(), ImGui::GetContentRegionAvail());

    // ����Ÿ�� ���󺹱�
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
    for (UINT i = 0; i < TEX_PARAM::TEX_END; i++)
    {
        CTexture::Clear(i);
    }

    ImGui::End();
}

void CMaterialEditor::DrawDetails()
{
    ImGui::Begin("Details##MaterialEditor");

    ImGui_InputText("Material Name", ToString(m_Mtrl->GetName()));

    ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
                                             ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;

    // Material Coefficient
    if (ImGui::TreeNodeEx("Material Coefficient##MaterialEditor", DefaultTreeNodeFlag, "Material Coefficient"))
    {
        tMtrlConst& MtrlConst = m_Mtrl->m_Const;

        ImGui::ColorEdit4(ImGui_LabelPrefix("Albedo").c_str(), &MtrlConst.mtrl.vAlbedo.x);
        ImGui::ColorEdit4(ImGui_LabelPrefix("Diffuse").c_str(), &MtrlConst.mtrl.vDiffuse.x);
        ImGui::ColorEdit4(ImGui_LabelPrefix("Specular").c_str(), &MtrlConst.mtrl.vSpecular.x);
        ImGui::SliderFloat(ImGui_LabelPrefix("Metallic").c_str(), &MtrlConst.mtrl.vMetallic, 0.f, 1.f);
        ImGui::SliderFloat(ImGui_LabelPrefix("Roughness").c_str(), &MtrlConst.mtrl.vRoughness, 0.f, 1.f);
        ImGui::ColorEdit4(ImGui_LabelPrefix("Emission").c_str(), &MtrlConst.mtrl.vEmission.x);

        ImGui::TreePop();
    }

    // Shader
    Ptr<CGraphicsShader> pShader = m_Mtrl->GetShader();

    if (ImGui::TreeNodeEx("Shader##MaterialEditor", DefaultTreeNodeFlag, "Shader"))
    {
        // Shader Name Combobox
        {
            string ShaderName = string();
            if (nullptr != pShader)
                ShaderName += ToString(pShader->GetName());

            const map<wstring, Ptr<CAsset>>& ShadersMap = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::GRAPHICS_SHADER);

            if (ImGui_ComboUI(ImGui_LabelPrefix("Shader Name").c_str(), ShaderName, ShadersMap))
            {
                m_Mtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(ToWstring(ShaderName)));
            }
        }

        ImGui::Separator();

        // Scaler Parameter
        if (nullptr != pShader)
        {
            const vector<tScalarParam>& ScalerParams = pShader->GetScalarParam();

            for (int i = 0; i < ScalerParams.size(); i++)
            {
                switch (ScalerParams[i].Type)
                {
                case INT_0:
                case INT_1:
                case INT_2:
                case INT_3: {
                    ImGui::DragInt(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (int*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                   ScalerParams[i].DragSpeed);
                }
                break;
                case FLOAT_0:
                case FLOAT_1:
                case FLOAT_2:
                case FLOAT_3: {
                    ImGui::DragFloat(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (float*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                     ScalerParams[i].DragSpeed);
                }
                break;
                case VEC2_0:
                case VEC2_1:
                case VEC2_2:
                case VEC2_3: {
                    ImGui::DragFloat2(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (float*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                      ScalerParams[i].DragSpeed);
                }
                break;
                case VEC4_0:
                case VEC4_1:
                case VEC4_2:
                case VEC4_3: {
                    ImGui::DragFloat4(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (float*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                      ScalerParams[i].DragSpeed);
                }
                break;
                case MAT_0:
                case MAT_1:
                case MAT_2:
                case MAT_3: {
                }
                break;
                }
            }

            ImGui::Separator();

            // Texture
            const vector<tTexParam>& TexParams = pShader->GetTexParam();
            constexpr float IMAGE_BASE_SIZE = 100.0f;

            for (int i = 0; i < TexParams.size(); i++)
            {
                Ptr<CTexture> pTex = m_Mtrl->GetTexParam(TexParams[i].Type);
                string CurTextureName = string();
                ID3D11ShaderResourceView* pSRV = nullptr;

                // Texture Render
                {
                    if (nullptr != pTex.Get())
                    {
                        pSRV = pTex->GetSRV().Get();
                        CurTextureName = ToString(pTex->GetName());
                    }
                    else
                        pSRV = CAssetMgr::GetInst()->Load<CTexture>(L"Texture/missing_texture.png", L"Texture/missing_texture.png")->GetSRV().Get();

                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, 250);
                    ImGui::Spacing();
                    ImGui::Dummy(ImVec2(0.f, 30.f));
                    ImGui::Text(TexParams[i].Desc.c_str());
                    ImGui::NextColumn();

                    ImGui::Image((void*)pSRV, ImVec2(IMAGE_BASE_SIZE, IMAGE_BASE_SIZE));
                }

                // Delete Texture Popup
                {
                    string PopupID = "Delete Texture##MaterialEditor";
                    PopupID += TexParams[i].Desc;
                    ImGui::OpenPopupOnItemClick(PopupID.c_str(), ImGuiPopupFlags_MouseButtonRight);

                    if (ImGui::BeginPopup(PopupID.c_str()))
                    {
                        if (ImGui::MenuItem("Delete Texture"))
                        {
                            m_Mtrl->SetTexParam(TexParams[i].Type, nullptr);
                        }

                        ImGui::EndPopup();
                    }
                }

                ImGui::SameLine();

                // Textures Combobox
                if (ImGui_TexturesComboUI(string("##MaterialEditorTextures" + TexParams[i].Desc).c_str(), CurTextureName))
                {
                    m_Mtrl->SetTexParam(TexParams[i].Type, CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(CurTextureName)));
                }

                ImGui::Columns(1);
            }
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

void CMaterialEditor::tick()
{
    m_ViewportCam->GetOwner()->tick();
    m_ViewportObj->tick();

    m_LightObj->Transform()->tick();
}

void CMaterialEditor::finaltick()
{
    m_ViewportCam->GetOwner()->finaltick();
    m_ViewportObj->finaltick();

    // Light Register ��� X
    m_LightObj->Transform()->finaltick();
}

void CMaterialEditor::render(bool* open)
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

    ImGuiID dockSpace = ImGui::GetID("Material Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // Material Editor Render
    // =====================================
    render();

    ImGui::End();
}
