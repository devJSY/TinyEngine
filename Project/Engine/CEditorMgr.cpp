#include "pch.h"
#include "CEditorMgr.h"
#include "CEditor.h"
#include "CRenderMgr.h"
#include <Scripts\\CScriptMgr.h>
#include "CAssetMgr.h"
#include "CLevelMgr.h"

#include "CLevelEditor.h"

#include "CEngine.h"
#include "CDevice.h"
#include "CGameObjectEx.h"
#include "components.h"
#include "CScript.h"
#include "CMRT.h"

CEditorMgr::CEditorMgr()
    : m_bEnabled(true)
    , m_arrEditor{}
    , m_SelectedObj(nullptr)
    , m_ViewportSize(Vec2())
    , m_ViewportMousePos(Vec2())
    , m_hObserver(nullptr)
{
#ifdef DISTRIBUTE
    m_bEnabled = false;
#endif // DISTRIBUTE
}

CEditorMgr::~CEditorMgr()
{
    if (!m_bEnabled)
        return;

    Delete_Array(m_arrEditor);

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Editor Objects
    Delete_Vec(m_vecEditorObj);

    // 디렉터리 변경사항 확인 종료
    FindCloseChangeNotification(m_hObserver);
}

void CEditorMgr::init()
{
    if (!m_bEnabled)
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

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
    // ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Rounding
    style.TabRounding = 5.f;
    style.FrameRounding = 8.f;
    style.GrabRounding = 8.f;
    style.WindowRounding = 8.f;
    style.PopupRounding = 8.f;

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

    m_arrEditor[(UINT)EDITOR_TYPE::MODEL] = new CModelEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::MODEL]->SetName(L"Model Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::MODEL]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::TILEMAP] = new CTileMapEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::TILEMAP]->SetName(L"TileMap Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::TILEMAP]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::PHYSICS2D_MATERIAL] = new CPhysics2DMaterialEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::PHYSICS2D_MATERIAL]->SetName(L"Physics2D Material Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::PHYSICS2D_MATERIAL]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::PHYSIC_MATERIAL] = new CPhysicMaterialEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::PHYSIC_MATERIAL]->SetName(L"Physic Material Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::PHYSIC_MATERIAL]->init();

    m_arrEditor[(UINT)EDITOR_TYPE::SOUND] = new CSoundEditor;
    m_arrEditor[(UINT)EDITOR_TYPE::SOUND]->SetName(L"Sound Editor");
    m_arrEditor[(UINT)EDITOR_TYPE::SOUND]->init();

    // 에디터용 카메라 오브젝트 생성
    CGameObjectEx* pEditorCam = new CGameObjectEx;
    pEditorCam->SetName(L"Editor Camera");
    pEditorCam->AddComponent(new CTransform);
    pEditorCam->AddComponent(new CCamera);
    pEditorCam->AddComponent(CScriptMgr::GetScript(CAMERAMOVESCRIPT));

    pEditorCam->Camera()->LayerMaskAll();
    pEditorCam->Camera()->LayerMask(CLevelMgr::GetInst()->GetCurrentLevel(), L"UI", false);
    pEditorCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
    pEditorCam->Camera()->SetHDRI(true);
    pEditorCam->Camera()->SetCameraPriority(0);
    pEditorCam->Camera()->SetFOV(XMConvertToRadians(60.f));
    pEditorCam->Camera()->SetFar(50000.f);

    m_vecEditorObj.push_back(pEditorCam);

    // Editor 용 카메라로서 렌더매니저에 등록
    CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCam->Camera());

    // Editor 모드에선 컨텐츠 폴더에 존재하는 모든 에셋 로딩
    //CAssetMgr::GetInst()->ReloadContent();

    // Content 폴더 변경사항 확인용 핸들 초기화
    wstring strContentPath = CPathMgr::GetContentPath();
    m_hObserver = FindFirstChangeNotification(strContentPath.c_str(), true,
                                              FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_ACTION_ADDED | FILE_ACTION_REMOVED);
}

void CEditorMgr::tick()
{
    if (!m_bEnabled)
        return;

    // ======================
    // Editor Tick
    // ======================
    m_arrEditor[(UINT)EDITOR_TYPE::LEVEL]->tick();
    m_arrEditor[(UINT)EDITOR_TYPE::LEVEL]->finaltick();

    // ======================
    // Editor Object Tick
    // ======================
    for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
    {
        m_vecEditorObj[i]->tick();
    }

    for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
    {
        m_vecEditorObj[i]->finaltick();
    }

    // Content 폴더 변경사항 확인
    //ObserveContent();
}

void CEditorMgr::render()
{
    if (!m_bEnabled)
        return;

    // MRT Set
    CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

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
    GetLevelEditor()->render();

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

    style->TranslationLineThickness = 6.0f;
    style->TranslationLineArrowSize = 12.0f;
    style->RotationLineThickness = 6.0f;
    style->RotationOuterLineThickness = 6.0f;
    style->ScaleLineThickness = 6.0f;
    style->ScaleLineCircleSize = 10.0f;
    style->HatchedAxisLineThickness = 6.0f;
    style->CenterCircleSize = 6.0f;

    style->Colors[ImGuizmo::DIRECTION_X] = ImGui::ColorConvertU32ToFloat4(0xFF715ED8);
    style->Colors[ImGuizmo::DIRECTION_Y] = ImGui::ColorConvertU32ToFloat4(0xFF25AA25);
    style->Colors[ImGuizmo::DIRECTION_Z] = ImGui::ColorConvertU32ToFloat4(0xFFCC532C);

    style->Colors[ImGuizmo::PLANE_X] = ImGui::ColorConvertU32ToFloat4(0xFF7A68D8);
    style->Colors[ImGuizmo::PLANE_Y] = ImGui::ColorConvertU32ToFloat4(0xFF55AB55);
    style->Colors[ImGuizmo::PLANE_Z] = ImGui::ColorConvertU32ToFloat4(0xFFD96742);

    style->Colors[ImGuizmo::SELECTION] = ImGui::ColorConvertU32ToFloat4(0xFF20AACC);
    style->Colors[ImGuizmo::INACTIVE] = ImGui::ColorConvertU32ToFloat4(0x99999999);
    style->Colors[ImGuizmo::TRANSLATION_LINE] = ImGui::ColorConvertU32ToFloat4(0xAAAAAAAA);
    style->Colors[ImGuizmo::SCALE_LINE] = ImGui::ColorConvertU32ToFloat4(0xFF404040);
}

void CEditorMgr::ObserveContent()
{
    DWORD dwWaitStatus = WaitForSingleObject(m_hObserver, 0);

    if (dwWaitStatus == WAIT_OBJECT_0)
    {
        FindNextChangeNotification(m_hObserver);

        CAssetMgr::GetInst()->ReloadContent();
        LOG(Log, "Assets Reloaded!!");
    }
}
