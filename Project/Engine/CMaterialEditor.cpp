#include "pch.h"
#include "CMaterialEditor.h"
#include "CEditorMgr.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "components.h"
#include "CGameObjectEx.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CMaterialEditor::CMaterialEditor()
    : CEditor(EDITOR_TYPE::MATERIAL)
    , m_Mtrl()
    , m_ViewportRTTex(nullptr)
    , m_ViewportDSTex(nullptr)
    , m_ViewportCam(nullptr)
    , m_ViewportObj(nullptr)
    , m_LightObj(nullptr)
    , m_LightBuffer(nullptr)
{
    // RenderTarget
    m_ViewportRTTex = CAssetMgr::GetInst()->CreateTexture(L"MtrlEditorViewportTex", 1280, 1280, DXGI_FORMAT_R16G16B16A16_FLOAT,
                                                          D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

    // DepthStencil 용도 텍스쳐 생성
    m_ViewportDSTex = CAssetMgr::GetInst()->CreateTexture(L"MtrlEditorViewportDSTex", 1280, 1280, DXGI_FORMAT_D24_UNORM_S8_UINT,
                                                          D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);

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
    m_ViewportCam->SetCameraPriority(0);

    // Object
    m_ViewportObj = new CGameObjectEx;
    m_ViewportObj->AddComponent(new CTransform);
    m_ViewportObj->AddComponent(new CMeshRender);

    m_ViewportObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
    m_ViewportObj->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 250.f));
    m_ViewportObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));

    // Light
    m_LightObj = new CGameObjectEx;
    m_LightObj->AddComponent(new CTransform);
    m_LightObj->AddComponent(new CLight3D);

    m_LightObj->Transform()->SetRelativePos(Vec3(250.f, 500.f, 0.f));
    m_LightObj->Light3D()->SetLightType(LIGHT_TYPE::POINT);
    m_LightObj->Light3D()->SetLightRadiance(Vec3(2.f, 2.f, 2.f));
    m_LightObj->Light3D()->SetRadius(10.f);
    m_LightObj->Light3D()->SetShadowType(0);

    m_LightBuffer = new CStructuredBuffer;
    m_LightBuffer->Create(sizeof(tLightInfo), 1, SB_TYPE::READ_ONLY, true);
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

void CMaterialEditor::render()
{
    if (nullptr == m_Mtrl.Get())
        return;

    ImGui_SetWindowClass_MaterialEditor();
    DrawViewport();

    ImGui_SetWindowClass_MaterialEditor();
    DrawDetails();
}

void CMaterialEditor::DrawViewport()
{
    ImGui::Begin("Viewport##MaterialEditor");

    // 렌더타겟 설정
    CONTEXT->ClearRenderTargetView(m_ViewportRTTex->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 1.f));
    CONTEXT->ClearDepthStencilView(m_ViewportDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    CONTEXT->OMSetRenderTargets(1, m_ViewportRTTex->GetRTV().GetAddressOf(), m_ViewportDSTex->GetDSV().Get());
    CDevice::GetInst()->SetViewport(1280.f, 1280.f);

    // 카메라 정보 설정
    g_Transform.matView = m_ViewportCam->GetViewMat();
    g_Transform.matViewInv = g_Transform.matView.Invert();
    g_Transform.matProj = m_ViewportCam->GetProjMat();
    g_Transform.matProjInv = g_Transform.matProj.Invert();

    // 광원 정보 설정
    g_Global.Light3DCount = 1;

    // 메인 카메라 위치 등록
    g_Global.eyeWorld = m_ViewportCam->Transform()->GetWorldPos();

    // 전역 상수 데이터 바인딩
    CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
    pGlobalBuffer->SetData(&g_Global);
    pGlobalBuffer->UpdateData();
    pGlobalBuffer->UpdateData_CS();

    // Light 정보 설정
    const tLightInfo& info = m_LightObj->Light3D()->GetLightInfo();

    tLightInfo CopyInfo = {};
    CopyInfo = info;
    CopyInfo.vWorldPos = m_LightObj->Transform()->GetWorldPos();
    CopyInfo.vWorldDir = m_LightObj->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    CopyInfo.viewMat = Matrix();
    CopyInfo.projMat = Matrix();
    CopyInfo.invProj = Matrix();

    m_LightBuffer->SetData(&CopyInfo, 1);
    m_LightBuffer->UpdateData(12);

    // 렌더링
    m_ViewportObj->MeshRender()->SetMaterial(m_Mtrl);
    m_ViewportObj->render();

    ImGui::Image((void*)m_ViewportRTTex->GetSRV().Get(), ImGui::GetContentRegionAvail());

    // 렌더타겟 원상복귀
    CDevice::GetInst()->SetViewport();
    CDevice::GetInst()->SetRenderTarget();

    ImGui::End();
}

void CMaterialEditor::DrawDetails()
{
    ImGui::Begin("Details##MaterialEditor");

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    string MtrlName = ToString(m_Mtrl->GetName());
    strcpy_s(buffer, sizeof(buffer), MtrlName.c_str());
    if (ImGui::InputText(ImGui_LabelPrefix("Material Name").c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
        m_Mtrl->SetName(ToWstring(buffer));

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
        string ShaderName = string();
        if (nullptr != pShader)
            ShaderName += ToString(pShader->GetName());

        const map<wstring, Ptr<CAsset>>& ShadersMap = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::GRAPHICS_SHADER);

        if (ImGui_ComboUI(ImGui_LabelPrefix("Shader Name").c_str(), ShaderName, ShadersMap))
        {
            m_Mtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(ToWstring(ShaderName)));
        }

        ImGui::Separator();

        if (nullptr != pShader)
        {
            // Scaler
            static float fDragSpeed = 1.0f;
            ImGui::SliderFloat(ImGui_LabelPrefix("Drag Speed").c_str(), &fDragSpeed, 0.f, 1.f);

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
                                   fDragSpeed);
                }
                break;
                case FLOAT_0:
                case FLOAT_1:
                case FLOAT_2:
                case FLOAT_3: {
                    ImGui::DragFloat(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (float*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                     fDragSpeed);
                }
                break;
                case VEC2_0:
                case VEC2_1:
                case VEC2_2:
                case VEC2_3: {
                    ImGui::DragFloat2(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (float*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                      fDragSpeed);
                }
                break;
                case VEC4_0:
                case VEC4_1:
                case VEC4_2:
                case VEC4_3: {
                    ImGui::DragFloat4(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (float*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                      fDragSpeed);
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
            const map<wstring, Ptr<CAsset>>& TexturesMap = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::TEXTURE);
            const vector<tTexParam>& TexParams = pShader->GetTexParam();
            constexpr float IMAGE_BASE_SIZE = 100.0f;

            for (int i = 0; i < TexParams.size(); i++)
            {
                Ptr<CTexture> pTex = m_Mtrl->GetTexParam(TexParams[i].Type);
                std::filesystem::path TextureName = std::filesystem::path();
                ID3D11ShaderResourceView* pSRV = nullptr;

                // Texture Render
                {
                    if (nullptr != pTex.Get())
                    {
                        pSRV = pTex->GetSRV().Get();
                        TextureName = pTex->GetName();
                        TextureName = TextureName.filename();
                    }
                    else
                        pSRV = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\missing_texture.png", L"Texture\\missing_texture.png")->GetSRV().Get();

                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, 300);
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
                {
                    if (ImGui::BeginCombo(string("##MaterialEditorTextures" + TexParams[i].Desc).c_str(), TextureName.string().c_str()))
                    {
                        for (const auto& iter : TexturesMap)
                        {
                            std::filesystem::path Key = iter.first;
                            Key = Key.filename();
                            bool is_selected = (TextureName == Key);
                            if (ImGui::Selectable(Key.string().c_str(), is_selected))
                            {
                                Ptr<CAsset> tex = iter.second;
                                m_Mtrl->SetTexParam(TexParams[i].Type, (CTexture*)tex.Get());
                            }

                            if (is_selected)
                            {
                                ImGui::SetItemDefaultFocus();
                            }

                            // Tooltip
                            if (ImGui::IsItemHovered())
                            {
                                if (ImGui::BeginItemTooltip())
                                {
                                    Ptr<CAsset> HoveredTex = iter.second;
                                    ImGui::Text(ToString(HoveredTex->GetName()).c_str());
                                    ImGui::Image(((CTexture*)HoveredTex.Get())->GetSRV().Get(), ImVec2(100, 100));
                                    ImGui::EndTooltip();
                                }
                            }
                        }

                        ImGui::EndCombo();
                    }
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

    // Light Register 등록 X
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
