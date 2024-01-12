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
#include "CAssetMgr.h"
#include "CCollisionMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CLevelSaveLoad.h"

CLevelEditor::CLevelEditor()
    : CEditor(EDITOR_TYPE::LEVEL)
    , m_ViewportFocused(false)
    , m_ViewportHovered(false)
    , m_bShowIDMap(false)
    , m_Outliner()
    , m_GizmoType(ImGuizmo::OPERATION::TRANSLATE)
{
}

CLevelEditor::~CLevelEditor()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void CLevelEditor::init()
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

    m_Outliner.init();
    m_ContentBrowser.init();
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

void CLevelEditor::render()
{
    // Viewport
    render_Viewport();

    // ID Map
    if (m_bShowIDMap)
    {
        Ptr<CTexture> pIDMap = CAssetMgr::GetInst()->FindAsset<CTexture>(L"IDMapTex");
        ImGui::Begin("Picking Color ID Map");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ImGui::Image((void*)pIDMap->GetSRV().Get(), ImVec2(viewportPanelSize.x, viewportPanelSize.y));
        ImGui::End();
    }

    // Menu Bar
    render_MenuBar();

    // Collision Responses
    render_CollisionResponses();

    // Outliner Render
    m_Outliner.render();

    // ContentBrowser Render
    m_ContentBrowser.render();

    // UI Toolbar
    render_UI_Toolbar();

    // Assets
    render_Assets();

    // Demo ImGUI Rendering
    bool show_demo_window = true;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // World Settings
    ImGui::Begin("World Settings");
    ImGui::Text("FPS : %d", CTimeMgr::GetInst()->GetFPS());
    ImGui::Text("Delta Time : %.5f", CTimeMgr::GetInst()->GetDeltaTime());

    ImGui::Text("Choice Your Clear Color!");
    ImGui::ColorPicker3("clear color", (float*)&CEngine::GetInst()->GetClearColor(),
                        ImGuiColorEditFlags_PickerHueWheel);
    ImGui::End();

    // View Mode
    ImGui::Begin("View Mode");
    ImGui::Checkbox("Picking Color ID Map", &m_bShowIDMap);
    ImGui::Checkbox("Draw WireFrame", (bool*)&g_Global.DrawAsWireFrame);

    bool bDebugRender = CRenderMgr::GetInst()->IsShowDebugRender();
    ImGui::Checkbox("Show DebugRender", &bDebugRender);
    CRenderMgr::GetInst()->SetShowDebugRender(bDebugRender);

    bool bCollider = CRenderMgr::GetInst()->IsShowCollider();
    ImGui::Checkbox("Show Collider", &bCollider);
    CRenderMgr::GetInst()->SetShowCollider(bCollider);
    ImGui::End();

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

void CLevelEditor::render_MenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Level"))
            {
                std::filesystem::path filePath = SaveFile(L"Maps\\", TEXT("레벨 파일\0*.tmap\0모든 파일(*.*)\0*.*\0"));

                wstring FileName = filePath.filename();
                if (!FileName.empty())
                {
                    // Level 저장
                    CLevelSaveLoad::SaveLevel(FileName, CLevelMgr::GetInst()->GetCurrentLevel());
                }
            }

            if (ImGui::MenuItem("Load Level"))
            {
                std::filesystem::path filePath = OpenFile(L"Maps\\", TEXT("레벨 파일\0*.tmap\0모든 파일(*.*)\0*.*\0"));

                wstring FileName = filePath.filename();
                if (!FileName.empty())
                {
                    // Level 불러오기
                    CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(FileName);

                    if (nullptr != pLoadedLevel)
                        GamePlayStatic::LevelChange(pLoadedLevel);
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void CLevelEditor::render_UI_Toolbar()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    ImGui::Begin("##toolbar", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    bool m_ActiveScene = true; // temp
    bool toolbarEnabled = (bool)m_ActiveScene;

    ImVec4 tintColor = ImVec4(1, 1, 1, 1);
    if (!toolbarEnabled)
        tintColor.w = 0.5f;

    float size = ImGui::GetWindowHeight() - 4.0f;
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

    bool hasPlayButton = true;
    bool hasSimulateButton = true;
    bool hasPauseButton = true;

    Ptr<CTexture> pPauseButtonTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PauseButtonTex");
    Ptr<CTexture> pPlayButtonTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PlayButtonTex");
    Ptr<CTexture> pSimulateButtonTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"SimulateButtonTex");
    Ptr<CTexture> pStepButtonTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"StepButtonTex");
    Ptr<CTexture> pStopButtonTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"StopButtonTex");

    if (hasPlayButton)
    {
        if (ImGui::ImageButton((void*)pPlayButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0,
                               ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) &&
            toolbarEnabled)
        {
        }
    }

    if (hasSimulateButton)
    {
        if (hasPlayButton)
            ImGui::SameLine();

        if (ImGui::ImageButton((void*)pSimulateButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0),
                               ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) &&
            toolbarEnabled)
        {
        }
    }

    if (hasPauseButton)
    {
        ImGui::SameLine();
        {
            if (ImGui::ImageButton((void*)pPauseButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0),
                                   ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) &&
                toolbarEnabled)
            {
            }
        }

        // Step button
        ImGui::SameLine();
        {
            if (ImGui::ImageButton((void*)pStepButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0),
                                   ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) &&
                toolbarEnabled)
            {
            }
        }
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();
}

void CLevelEditor::render_Assets()
{
    ImGui::Begin("Assets");

    for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
    {
        const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetMapAsset((ASSET_TYPE)i);

        if (ImGui::TreeNodeEx(GetAssetTypeName((ASSET_TYPE)i).c_str(),
                              ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                                  ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
        {
            for (const auto& iter : mapAsset)
            {
                string key = WstringTostring(iter.first);

                ImGui::TreeNodeEx(key.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);

                // Drag & Drop
                if (ImGui::BeginDragDropSource())
                {
                    ImGui::Text("%s", key.c_str(), key.size());

                    ImGui::SetDragDropPayload("ASSETS", key.c_str(), key.size());
                    ImGui::EndDragDropSource();
                }
            }

            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void CLevelEditor::render_Viewport()
{
    // RT Copy
    CRenderMgr::GetInst()->CopyRenderTarget();

    ImGui::Begin("Level ViewPort");

    // Viewport에서의 마우스 위치 등록
    ImVec2 viewportPos = ImGui::GetCursorScreenPos();
    CEditorMgr::GetInst()->SetViewportMousePos(
        Vec2(ImGui::GetIO().MousePos.x - viewportPos.x, ImGui::GetIO().MousePos.y - viewportPos.y));

    // 상태 확인
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    // 크기 등록
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    CEditorMgr::GetInst()->SetViewportSize(Vec2(viewportSize.x, viewportSize.y));

    // 렌더링
    Ptr<CTexture> pCopyTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RTCopyTex");
    ImGui::Image((void*)pCopyTex->GetSRV().Get(), viewportSize);

    // ImGuizmo
    render_ImGuizmo();

    // Drag & Drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            string name = (char*)payload->Data;
            name.resize(payload->DataSize);
            std::filesystem::path fileNameStr = name;
            if (fileNameStr.extension() == CLevelSaveLoad::GetLevelExtension())
            {
                // Level 불러오기
                CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(stringToWstring(name));

                if (nullptr != pLoadedLevel)
                    GamePlayStatic::LevelChange(pLoadedLevel);
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

void CLevelEditor::render_ImGuizmo()
{
    CGameObject* pSelectedObj = CLevelMgr::GetInst()->GetSelectedObject();
    CCamera* pCam = CRenderMgr::GetInst()->GetCamera(0); // Main Camera
    if (nullptr == pSelectedObj || nullptr == pCam)
        return;

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

    ImGuizmo::SetOrthographic(pCam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC ? true : false);

    ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    Matrix CamView = pCam->GetViewMat();
    Matrix CamProj = pCam->GetProjMat();

    // transform
    CTransform* pTr = pSelectedObj->Transform();
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
        Matrix ParentInvMat = pTr->GetParentMat().Invert();
        WorldMat *= ParentInvMat;
        originWorldMat *= ParentInvMat;

        // ImGuizmo변화량이 적용된 Matrix와 원본 Matrix SRT 분해
        float Ftranslation[3] = {0.0f, 0.0f, 0.0f}, Frotation[3] = {0.0f, 0.0f, 0.0f}, Fscale[3] = {0.0f, 0.0f, 0.0f};
        ImGuizmo::DecomposeMatrixToComponents(*WorldMat.m, Ftranslation, Frotation, Fscale);

        float originFtranslation[3] = {0.0f, 0.0f, 0.0f}, originFrotation[3] = {0.0f, 0.0f, 0.0f},
              originFscale[3] = {0.0f, 0.0f, 0.0f};
        ImGuizmo::DecomposeMatrixToComponents(*originWorldMat.m, originFtranslation, originFrotation, originFscale);

        // ImGuizmo로 조정한 변화량 추출
        Vec3 vPosOffset = Vec3(originFtranslation[0] - Ftranslation[0], originFtranslation[1] - Ftranslation[1],
                               originFtranslation[2] - Ftranslation[2]);
        Vec3 vRotOffset =
            Vec3(DirectX::XMConvertToRadians(originFrotation[0]) - DirectX::XMConvertToRadians(Frotation[0]),
                 DirectX::XMConvertToRadians(originFrotation[1]) - DirectX::XMConvertToRadians(Frotation[1]),
                 DirectX::XMConvertToRadians(originFrotation[2]) - DirectX::XMConvertToRadians(Frotation[2]));
        Vec3 vScaleOffset = Vec3(originFscale[0] - Fscale[0], originFscale[1] - Fscale[1], originFscale[2] - Fscale[2]);

        // 부모 ↔ 자식 계층 구조이기 때문에 변화량을 계산해서 적용
        pTr->SetRelativePos(pTr->GetRelativePos() - vPosOffset);
        pTr->SetRelativeRotation(pTr->GetRelativeRotation() - vRotOffset);
        pTr->SetRelativeScale(pTr->GetRelativeScale() - vScaleOffset);
    }
}

void CLevelEditor::render_CollisionResponses()
{
    ImGui::Begin("Collision Responses");

    if (ImGui::Button("All Layer Enable"))
        CCollisionMgr::GetInst()->EnableAllLayer();

    if (ImGui::Button("All Layer Disable"))
        CCollisionMgr::GetInst()->DisableAllLayer();

    string column_names[LAYER_MAX + 1] = {"Layers"};
    for (int i = 1; i <= LAYER_MAX; i++)
    {
        const wstring& LayerName = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i - 1)->GetName();
        column_names[i] = WstringTostring(LayerName);
    }

    const int columns_count = LAYER_MAX + 1; // columns 0 is Layer Name

    static bool bools[columns_count * LAYER_MAX] = {}; // Dummy storage selection storage
    for (UINT iRow = 0; iRow < LAYER_MAX; ++iRow)
    {
        for (UINT iCol = iRow; iCol < LAYER_MAX; ++iCol)
        {
            if (CCollisionMgr::GetInst()->GetCollisionLayer(iRow) & (1 << iCol))
            {
                bools[iRow * columns_count + iCol + 1] = true;
            }
            else
            {
                bools[iRow * columns_count + iCol + 1] = false;
            }
        }
    }

    static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX |
                                         ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter |
                                         ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable |
                                         ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn;

    if (ImGui::BeginTable("CollisionResponses", columns_count, table_flags, ImGui::GetContentRegionAvail()))
    {
        ImGui::TableSetupColumn(column_names[0].c_str(),
                                ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
        for (int n = columns_count - 1; n >= 1; n--)
            ImGui::TableSetupColumn(column_names[n].c_str(),
                                    ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed);

        ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the
                                        // ImGuiTableColumnFlags_AngledHeader flag.
        ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other functions.
        for (int row = 0; row < LAYER_MAX; row++)
        {
            ImGui::PushID(row);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::Text(column_names[row + 1].c_str());
            for (int column = columns_count - 1; column > row; column--)
                if (ImGui::TableSetColumnIndex(columns_count - column))
                {
                    ImGui::PushID(column);
                    if (ImGui::Checkbox("", &bools[row * columns_count + column]))
                        CCollisionMgr::GetInst()->LayerCheck(row, column - 1, bools[row * columns_count + column]);
                    ImGui::PopID();
                }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    ImGui::End();
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
