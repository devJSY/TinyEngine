#include "pch.h"
#include "CLevelEditor.h"
#include "CEngine.h"
#include "CDevice.h"

#include "CTaskMgr.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLevelMgr.h"
#include "CKeyMgr.h"

#include "CEditorMgr.h"
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CRenderMgr.h"

CLevelEditor::CLevelEditor()
    : CEditor(EDITOR_TYPE::LEVEL)
    , m_ViewportFocused(false)
    , m_ViewportHovered(false)
    , m_show_Viewport2(false)
    , m_Outliner()
    , m_GizmoType(ImGuizmo::OPERATION::TRANSLATE)
    , m_FontSize(20.f)
{
}

CLevelEditor::~CLevelEditor()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (nullptr != m_RTCopyTex)
    {
        m_RTCopyTex = nullptr;
    }
}

void CLevelEditor::begin()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
    //  io.ConfigViewportsNoAutoMerge = true;
    //  io.ConfigViewportsNoTaskBarIcon = true;
    //  io.ConfigViewportsNoDefaultParent = true;
    //  io.ConfigDockingAlwaysTabBar = true;
    //  io.ConfigDockingTransparentPayload = true;
    //  io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T
    //  WORK AS EXPECTED. DON'T USE IN USER APP! io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; //
    //  FIXME-DPI: Experimental.

    wstring wBold = CPathMgr::GetContentPath();
    wstring wRegular = CPathMgr::GetContentPath();

    wBold += L"fonts\\opensans\\OpenSans-Bold.ttf";
    wRegular += L"fonts\\opensans\\OpenSans-Regular.ttf";

    io.Fonts->AddFontFromFileTTF(WstringTostring(wBold).c_str(), m_FontSize);
    io.FontDefault = io.Fonts->AddFontFromFileTTF(WstringTostring(wRegular).c_str(), m_FontSize);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
    // ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // 테마 설정
    SetDarkThemeColors();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(CEngine::GetInst()->GetMainWind());
    ImGui_ImplDX11_Init(CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());

    // Vieport 텍스춰 생성
    CreateViewport();

    m_Outliner.begin();
    m_ContentBrowser.begin();
}

void CLevelEditor::tick()
{
    // ImGUI Render Start
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGuizmo::BeginFrame();
}

void CLevelEditor::finaltick()
{
    // ImGUI Rendering
    bool show_demo_window = true;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Begin("World Settings");
    ImGui::Text("FPS : %d", CTimeMgr::GetInst()->GetFPS());
    ImGui::Text("Delta Time : %.5f", CTimeMgr::GetInst()->GetDeltaTime());

    ImGui::Text("Choice Your Clear Color!");
    ImGui::ColorPicker3("clear color", (float*)&CEngine::GetInst()->GetClearColor(),
                        ImGuiColorEditFlags_PickerHueWheel);
    ImGui::End();

    ImGui::Begin("View Mode");
    ImGui::Checkbox("Another Viewport", &m_show_Viewport2);
    ImGui::Checkbox("WireFrame", &g_Global.DrawAsWireFrame);
    ImGui::End();
}

void CLevelEditor::render()
{
    // Viewport 복사
    CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), CDevice::GetInst()->GetRenderTargetTexture().Get());

    // Viewport
    ImGui::Begin("Level ViewPort");

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    CEditorMgr::GetInst()->SetViewportSize(Vec2(viewportSize.x, viewportSize.y));
    ImGui::Image((void*)m_RTCopyTex->GetSRV().Get(), viewportSize);

    // Drag & Drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            string name = (char*)payload->Data;
            name.resize(payload->DataSize);
            std::cout << name << std::endl;
        }

        ImGui::EndDragDropTarget();
    }

    // ImGuizmo
    CGameObject* SelectedObj = CLevelMgr::GetInst()->GetSelectedObj();
    if (nullptr != SelectedObj)
    {
        if (!KEY_PRESSED(KEY::RBTN))
        {
            // 선택된 오브젝트가 있을때 키입력으로 Gizmo 타입설정
            if (KEY_TAP(KEY::Q))
                m_GizmoType = (ImGuizmo::OPERATION)0;
            else if (KEY_TAP(KEY::W))
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            else if (KEY_TAP(KEY::E))
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            else if (KEY_TAP(KEY::R))
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
        }

        // Editor Camera
        CCamera* pCam = CRenderMgr::GetInst()->GetCamera(0);

        if (nullptr != pCam)
        {
            ImGuizmo::SetOrthographic(pCam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC ? true : false);

            ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);
            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            Matrix CamView = pCam->GetViewMat();
            Matrix CamProj = pCam->GetProjMat();

            // transform
            CTransform* pTr = SelectedObj->Transform();
            Matrix WorldMat = pTr->GetWorldMat();

            // Snapping
            bool snap = KEY_PRESSED(KEY::LCTRL);

            float snapValue = 0.f;
            if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
                snapValue = 25.f;
            else if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 30.0f;
            else if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
                snapValue = 1.0f;

            float snapValues[3] = {snapValue, snapValue, snapValue};

            ImGuizmo::Manipulate(*CamView.m, *CamProj.m, m_GizmoType, ImGuizmo::LOCAL, *WorldMat.m, nullptr,
                                 snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                Matrix originWorldMat = pTr->GetWorldMat();

                // 부모행렬을 상쇄시켜 로컬 좌표계로 변경
                WorldMat *= pTr->GetParentMat().Invert();
                originWorldMat *= pTr->GetParentMat().Invert();

                // ImGuizmo변화량이 적용된 Matrix와 원본 Matrix SRT 분해
                float Ftranslation[3] = {0.0f, 0.0f, 0.0f}, Frotation[3] = {0.0f, 0.0f, 0.0f},
                      Fscale[3] = {0.0f, 0.0f, 0.0f};
                ImGuizmo::DecomposeMatrixToComponents(*WorldMat.m, Ftranslation, Frotation, Fscale);

                float originFtranslation[3] = {0.0f, 0.0f, 0.0f}, originFrotation[3] = {0.0f, 0.0f, 0.0f},
                      originFscale[3] = {0.0f, 0.0f, 0.0f};
                ImGuizmo::DecomposeMatrixToComponents(*originWorldMat.m, originFtranslation, originFrotation,
                                                      originFscale);

                // ImGuizmo로 조정한 변화량 추출
                Vec3 vPosOffset = Vec3(originFtranslation[0] - Ftranslation[0], originFtranslation[1] - Ftranslation[1],
                                       originFtranslation[2] - Ftranslation[2]);
                Vec3 vRotOffset =
                    Vec3(DirectX::XMConvertToRadians(originFrotation[0]) - DirectX::XMConvertToRadians(Frotation[0]),
                         DirectX::XMConvertToRadians(originFrotation[1]) - DirectX::XMConvertToRadians(Frotation[1]),
                         DirectX::XMConvertToRadians(originFrotation[2]) - DirectX::XMConvertToRadians(Frotation[2]));
                Vec3 vScaleOffset =
                    Vec3(originFscale[0] - Fscale[0], originFscale[1] - Fscale[1], originFscale[2] - Fscale[2]);

                // 부모 ↔ 자식 계층 구조이기 때문에 변화량을 계산해서 적용
                pTr->SetRelativePos(pTr->GetRelativePos() - vPosOffset);
                pTr->SetRelativeRotation(pTr->GetRelativeRotation() - vRotOffset);
                pTr->SetRelativeScale(pTr->GetRelativeScale() - vScaleOffset);
            }
        }
    }

    ImGui::End();

    if (m_show_Viewport2)
    {
        ImGui::Begin("Level ViewPort2");
        if (!m_ViewportFocused)
            m_ViewportFocused = ImGui::IsWindowFocused();
        if (!m_ViewportHovered)
            m_ViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ImGui::Image((void*)m_RTCopyTex->GetSRV().Get(), ImVec2(viewportPanelSize.x, viewportPanelSize.y));
        ImGui::End();
    }

    // Outliner Render
    m_Outliner.render();

    // ContentBrowser Render
    m_ContentBrowser.render();

    // Rendering
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO& io = ImGui::GetIO();
    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CLevelEditor::Resize()
{
    CreateViewport();
}

void CLevelEditor::CreateViewport()
{
    if (nullptr == m_RTCopyTex)
        m_RTCopyTex = new CTexture;

    ID3D11Texture2D* ptex = CDevice::GetInst()->GetRenderTargetTexture().Get();
    Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();

    // Create texture.
    D3D11_TEXTURE2D_DESC txtDesc = {};
    ptex->GetDesc(&txtDesc);

    txtDesc.Width = (UINT)Resolution.x;
    txtDesc.Height = (UINT)Resolution.y;
    txtDesc.Usage = D3D11_USAGE_DYNAMIC;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    txtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    m_RTCopyTex->Create((UINT)Resolution.x, (UINT)Resolution.y, txtDesc.Format, txtDesc.BindFlags, txtDesc.Usage);

    // m_RTCopyTex->GetTex2D().Reset();
    // m_RTCopyTex->GetSRV().Reset();
    // ID3D11Texture2D* tex = CDevice::GetInst()->GetRenderTargetTexture().Get();
    // Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();

    //// Create texture.
    // D3D11_TEXTURE2D_DESC txtDesc = {};
    // tex->GetDesc(&txtDesc);

    // txtDesc.Width = (UINT)Resolution.x;
    // txtDesc.Height = (UINT)Resolution.y;
    // txtDesc.Usage = D3D11_USAGE_DYNAMIC;
    // txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    // txtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    // DEVICE->CreateTexture2D(&txtDesc, nullptr, m_RTCopyTex->GetTex2D().GetAddressOf());
    // DEVICE->CreateShaderResourceView(m_RTCopyTex->GetTex2D().Get(), nullptr, m_RTCopyTex->GetSRV().GetAddressOf());
}

void CLevelEditor::SetDarkThemeColors()
{
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}
