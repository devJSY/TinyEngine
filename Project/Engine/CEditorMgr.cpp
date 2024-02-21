#include "pch.h"
#include "CEditorMgr.h"
#include "CEditor.h"
#include "CRenderMgr.h"

#include "CLevelEditor.h"

#include "CEngine.h"
#include "CDevice.h"
#include "CGameObjectEx.h"
#include "components.h"
#include "CCameraMoveScript.h"

CEditorMgr::CEditorMgr()
    : m_bEnable(true)
    , m_arrEditor{}
    , m_SelectedObj(nullptr)
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

    Delete_Vec(m_vecEditorObj);
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

    // Font Set
    float fFontSize = 25.f;
    wstring wBold = CPathMgr::GetContentPath();
    wstring wRegular = CPathMgr::GetContentPath();

    wBold += L"fonts\\opensans\\OpenSans-Bold.ttf";
    wRegular += L"fonts\\opensans\\OpenSans-Regular.ttf";

    io.Fonts->AddFontFromFileTTF(ToString(wBold).c_str(), fFontSize);
    io.FontDefault = io.Fonts->AddFontFromFileTTF(ToString(wRegular).c_str(), fFontSize);

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
    SetImGuizmoStyle();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(CEngine::GetInst()->GetMainWind());
    ImGui_ImplDX11_Init(CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());

    // ==================================
    // Editor 생성
    // ==================================
    m_arrEditor[(UINT)EDITOR_TYPE::LEVEL] = new CLevelEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::LEVEL]->SetName(L"Level Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::LEVEL]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::MATERIAL] = new CMaterialEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::MATERIAL]->SetName(L"Material Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::MATERIAL]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::BLUEPRINT] = new CBlueprintEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::BLUEPRINT]->SetName(L"Blueprint Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::BLUEPRINT]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::SPRITE] = new CSpriteEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::SPRITE]->SetName(L"Sprite Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::SPRITE]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::TILEMAP] = new CTileMapEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::TILEMAP]->SetName(L"TileMap Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::TILEMAP]->init();

    // 에디터용 카메라 오브젝트 생성
    CGameObjectEx* pEditorCam = new CGameObjectEx;
    pEditorCam->AddComponent(new CTransform);
    pEditorCam->AddComponent(new CCamera);
    pEditorCam->AddComponent(CScriptMgr::GetScript(SCRIPT_TYPE::CAMERAMOVESCRIPT));

    pEditorCam->Camera()->LayerMaskAll();
    pEditorCam->Camera()->LayerMask(31, false);
    pEditorCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
    pEditorCam->Camera()->SetFOV(XM_PI / 2.f);
    pEditorCam->Camera()->SetFar(100000.f);
    pEditorCam->Camera()->SetHDRI(true);
    pEditorCam->Camera()->SetCameraPriority(0);

    m_vecEditorObj.push_back(pEditorCam);

    // Editor 용 카메라로서 렌더매니저에 등록
    CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCam->Camera());
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
    }

    for (UINT i = 0; i < (UINT)EDITOR_TYPE::END; i++)
    {
        if (nullptr == m_arrEditor[i])
            continue;

        m_arrEditor[i]->finaltick();
    }

    for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
    {
        m_vecEditorObj[i]->tick();
    }

    for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
    {
        m_vecEditorObj[i]->finaltick();
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
    m_arrEditor[(UINT)EDITOR_TYPE::LEVEL]->render();

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

void CEditorMgr::SetImGuizmoStyle()
{
    ImGuizmo::Style* style = &ImGuizmo::GetStyle(); // get a pointer to the global style context
    ImVec4* colors = style->Colors;                 // get access to the colors array

    style->TranslationLineThickness = 3.0f;
    style->TranslationLineArrowSize = 8.0f;

    // lighter r/g/b colours
    colors[ImGuizmo::DIRECTION_X] = ImVec4(0.858f, 0.243f, 0.113f, 0.929f);
    colors[ImGuizmo::DIRECTION_Y] = ImVec4(0.603f, 0.952f, 0.282f, 0.929f);
    colors[ImGuizmo::DIRECTION_Z] = ImVec4(0.227f, 0.478f, 0.972f, 0.929f);
}
