#include "pch.h"
#include "CEditorMgr.h"
#include "CEditor.h"
#include "CLevelEditor.h"
#include "CMaterialEditor.h"
#include "CBlueprintEditor.h"

#include "CEngine.h"
#include "CDevice.h"

CEditorMgr::CEditorMgr()
    : m_bEnable(true)
    , m_arrEditor{}
    , m_ViewportSize(Vec2())
    , m_ViewportMousePos(Vec2())
{
}

CEditorMgr::~CEditorMgr()
{
    if (!m_bEnable)
        return;

    Delete_Array(m_arrEditor);

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void CEditorMgr::init()
{
    if (!m_bEnable)
        return;

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

    float fFontSize = 25.f;
    wstring wBold = CPathMgr::GetContentPath();
    wstring wRegular = CPathMgr::GetContentPath();

    wBold += L"fonts\\opensans\\OpenSans-Bold.ttf";
    wRegular += L"fonts\\opensans\\OpenSans-Regular.ttf";

    io.Fonts->AddFontFromFileTTF(WstringTostring(wBold).c_str(), fFontSize);
    io.FontDefault = io.Fonts->AddFontFromFileTTF(WstringTostring(wRegular).c_str(), fFontSize);

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

    // ==================================
    // Editor 생성
    // ==================================
    m_arrEditor[(UINT)EDITOR_TYPE::LEVEL] = new CLevelEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::LEVEL]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::MATERIAL] = new CMaterialEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::MATERIAL]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::BLUEPRINT] = new CBlueprintEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::BLUEPRINT]->init();
}

void CEditorMgr::tick()
{
    if (!m_bEnable)
        return;

    for (UINT i = 0; i < (UINT)EDITOR_TYPE::END; i++)
    {
        if (nullptr == m_arrEditor[i])
            continue;

        m_arrEditor[i]->tick();
        m_arrEditor[i]->finaltick();
    }
}

void CEditorMgr::render()
{
    if (!m_bEnable)
        return;

    // ====================
    // ImGUI Render Start
    // ====================
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGuizmo::BeginFrame();

    // ======================
    // Editor Render
    // ======================
    for (UINT i = 0; i < (UINT)EDITOR_TYPE::END; i++)
    {
        if (nullptr == m_arrEditor[i])
            continue;

        m_arrEditor[i]->render();
    }

    // ====================
    // ImGUI Rendering
    // ====================
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

void CEditorMgr::SetDarkThemeColors()
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
