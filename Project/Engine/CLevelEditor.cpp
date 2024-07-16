#include "pch.h"
#include "CLevelEditor.h"
#include "CEngine.h"

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
#include <Scripts\\CScriptMgr.h>
#include "CPhysics2DMgr.h"
#include "CPhysicsMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CScript.h"
#include "CMRT.h"

#include "CLevelSaveLoad.h"
#include "CMaterialEditor.h"
#include "CTileMapEditor.h"

CLevelEditor::CLevelEditor()
    : CEditor(EDITOR_TYPE::LEVEL)
    , m_ViewportFocused(false)
    , m_ViewportHovered(false)
    , m_Outliner()
    , m_ContentBrowser()
    , m_GizmoType(ImGuizmo::OPERATION::TRANSLATE)
    , m_bEnableGizmo(true)
    , m_bShowEditor{}
    , m_PlayButtonTex(nullptr)
    , m_SimulateButtonTex(nullptr)
    , m_StepButtonTex(nullptr)
    , m_PauseButtonTex(nullptr)
    , m_StopButtonTex(nullptr)
    , m_TranslateButtonTex(nullptr)
    , m_RotateButtonTex(nullptr)
    , m_ScaleButtonTex(nullptr)
    , m_ModalAssetType(ASSET_TYPE::END)
    , m_bShowWorldSettings(true)
    , m_bShowViewport(true)
    , m_bShowIDMap(false)
    , m_bShowOutliner(true)
    , m_bShowContentBrowser(false)
    , m_bShowToolbar(true)
    , m_bShowAssets(true)
    , m_bShowOutputLog(true)
    , m_bShowPhysics2DMgr(false)
    , m_bShowPhysicsMgr(false)
    , m_bShowTagsAndLayers(false)
    , m_bShowMRT(false)
    , m_bShowSSAO(false)
{
}

CLevelEditor::~CLevelEditor()
{
}

void CLevelEditor::init()
{
    m_Outliner.init();
    m_ContentBrowser.init();

    m_PlayButtonTex = CAssetMgr::GetInst()->Load<CTexture>(L"Icons\\PlayButton.png", L"Icons\\PlayButton.png");
    m_SimulateButtonTex = CAssetMgr::GetInst()->Load<CTexture>(L"Icons\\SimulateButton.png", L"Icons\\SimulateButton.png");
    m_StepButtonTex = CAssetMgr::GetInst()->Load<CTexture>(L"Icons\\StepButton.png", L"Icons\\StepButton.png");
    m_PauseButtonTex = CAssetMgr::GetInst()->Load<CTexture>(L"Icons\\PauseButton.png", L"Icons\\PauseButton.png");
    m_StopButtonTex = CAssetMgr::GetInst()->Load<CTexture>(L"Icons\\StopButton.png", L"Icons\\StopButton.png");
    m_TranslateButtonTex = CAssetMgr::GetInst()->Load<CTexture>(L"Icons\\translate.png", L"Icons\\translate.png");
    m_RotateButtonTex = CAssetMgr::GetInst()->Load<CTexture>(L"Icons\\rotate.png", L"Icons\\rotate.png");
    m_ScaleButtonTex = CAssetMgr::GetInst()->Load<CTexture>(L"Icons\\scale.png", L"Icons\\scale.png");

    COutputLog::GetInst()->init();
}

void CLevelEditor::tick()
{
    // =========================
    // Editor Tick
    // =========================
    for (UINT i = 0; i < (UINT)EDITOR_TYPE::END; i++)
    {
        if (i == (UINT)EDITOR_TYPE::LEVEL || !m_bShowEditor[i])
            continue;

        CEditorMgr::GetInst()->GetEditor((EDITOR_TYPE)i)->tick();
    }
}

void CLevelEditor::finaltick()
{
    static bool prevUsingGuizmo = false;

    CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();

    // Alt 키 누른상태로 기즈모 이동시 오브젝트 복사
    if (nullptr != pSelectedObj && !prevUsingGuizmo && ImGuizmo::IsUsing() && (ImGuizmo::OPERATION::TRANSLATE & m_GizmoType) &&
        (KEY_TAP(KEY::LALT) || KEY_PRESSED(KEY::LALT)))
    {
        GamePlayStatic::CloneGameObject(pSelectedObj);
    }

    prevUsingGuizmo = ImGuizmo::IsUsing();

    // =========================
    // Editor Final Tick
    // =========================
    for (UINT i = 0; i < (UINT)EDITOR_TYPE::END; i++)
    {
        if (i == (UINT)EDITOR_TYPE::LEVEL || !m_bShowEditor[i])
            continue;

        CEditorMgr::GetInst()->GetEditor((EDITOR_TYPE)i)->finaltick();
    }
}

void CLevelEditor::render()
{
    // Menu Bar
    render_MenuBar();

    // =====================================
    // DockSpace
    // =====================================
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetMainViewport()->ID;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplitOther;
    window_class.DockingAllowUnclassed = true;

    ImGui::SetNextWindowClass(&window_class);
    ImGui::Begin(ToString(GetName()).c_str());

    ImGuiID dockSpace = ImGui::GetID("Level Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // Destroy Object
    CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();
    if (ImGui::IsItemHovered() && nullptr != pSelectedObj && KEY_TAP(KEY::DEL))
    {
        GamePlayStatic::DestroyGameObject(pSelectedObj);
    }

    // =====================================
    // Level Editor Render
    // =====================================

    // World Settings
    if (m_bShowWorldSettings)
        render_WorldSettings();

    // Viewport
    if (m_bShowViewport)
        render_Viewport();

    // ID Map
    if (m_bShowIDMap)
    {
        ImGui_SetWindowClass(GetEditorType());
        ImGui::Begin("Picking Color ID Map", &m_bShowIDMap);
        ImGui::Image((void*)CRenderMgr::GetInst()->GetIDMapTex()->GetSRV().Get(), ImGui::GetContentRegionAvail());
        ImGui::End();
    }

    // Outliner Render
    if (m_bShowOutliner)
        m_Outliner.render();

    // ContentBrowser Render
    if (m_bShowContentBrowser)
        m_ContentBrowser.render();

    // UI Toolbar
    if (m_bShowToolbar)
        render_Toolbar();

    // Assets
    if (m_bShowAssets)
        render_Assets();

    // OutputLog
    if (m_bShowOutputLog)
        COutputLog::GetInst()->render(&m_bShowOutputLog);

    // Physics2DMgr
    if (m_bShowPhysics2DMgr)
        render_Physics2DMgr();

    // PhysicsMgr
    if (m_bShowPhysicsMgr)
        render_PhysicsMgr();

    // TagsAndLayers
    if (m_bShowTagsAndLayers)
        render_TagsAndLayers();

    // MRT
    if (m_bShowMRT)
        render_MRT();

    // SSAO
    if (m_bShowSSAO)
        render_SSAO();

    // Cam Preview
    render_CamPreview();

    //// ImGui Demo
    // static bool b = true;
    // ImGui::ShowDemoWindow(&b);

    if (CAssetMgr::GetInst()->IsModelLoading())
    {
        ImGui::Begin("Model Loading", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking);

        const ImU32 col = ImGui::GetColorU32(ImVec4{0.2f, 0.45f, 0.811f, 1.0f});
        const ImU32 bg = ImGui::GetColorU32(ImVec4{0.258f, 0.258f, 0.258f, 1.0f});

        ImGui_Spinner("##spinner", 15, 6, col);
        ImGui_BufferingBar("##buffer_bar", CAssetMgr::GetInst()->GetModelLoadingProgress(), ImVec2(400, 6), bg, col);

        ImGui::End();
    }

    ImGui::End(); // dockspace End

    // =========================
    // Editor Render
    // =========================
    for (UINT i = 0; i < (UINT)EDITOR_TYPE::END; i++)
    {
        if (i == (UINT)EDITOR_TYPE::LEVEL || !m_bShowEditor[i])
            continue;

        CEditorMgr::GetInst()->GetEditor((EDITOR_TYPE)i)->render(&m_bShowEditor[i]);
    }
}

void CLevelEditor::render_MenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        // FPS
        ImGui::TextColored(ImVec4(108.f / 255.f, 172.f / 255.f, 150.f / 255.f, 1.0f), "FPS : %d", CTimeMgr::GetInst()->GetFPS());

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Level"))
            {
                GamePlayStatic::ChangeLevel(CLevelMgr::GetInst()->CreateNewLevel(), LEVEL_STATE::STOP);
            }

            if (ImGui::MenuItem("New Default Kirby Level"))
            {
                GamePlayStatic::ChangeLevel(CLevelMgr::GetInst()->CreateDefaultKirbyLevel(), LEVEL_STATE::STOP);
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Save Level"))
            {
                std::filesystem::path filePath = SaveFileDialog(L"Levels\\", TEXT("레벨 파일\0*.tLevel\0모든 파일(*.*)\0*.*\0"));

                wstring FileName = filePath.filename();
                if (!FileName.empty())
                {
                    // Level 저장
                    CLevelSaveLoad::SaveLevel(CLevelMgr::GetInst()->GetCurrentLevel(), FileName);
                }
            }

            if (ImGui::MenuItem("Load Level"))
            {
                std::filesystem::path filePath = OpenFileDialog(L"Levels\\", TEXT("레벨 파일\0*.tLevel\0모든 파일(*.*)\0*.*\0"));

                wstring FileName = filePath.filename();
                if (!FileName.empty())
                {
                    // Level 불러오기
                    CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(FileName);

                    if (nullptr != pLoadedLevel)
                        GamePlayStatic::ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Editor"))
        {
            for (UINT i = 0; i < (UINT)EDITOR_TYPE::END; i++)
            {
                if (i == (UINT)EDITOR_TYPE::LEVEL)
                    continue;

                if (ImGui::MenuItem(ToString(CEditorMgr::GetInst()->GetEditor((EDITOR_TYPE)i)->GetName()).c_str(), NULL, m_bShowEditor[i]))
                    m_bShowEditor[i] = !m_bShowEditor[i];
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("World Settings", NULL, m_bShowWorldSettings))
                m_bShowWorldSettings = !m_bShowWorldSettings;

            if (ImGui::MenuItem("Viewport", NULL, m_bShowViewport))
                m_bShowViewport = !m_bShowViewport;

            if (ImGui::MenuItem("IDMap", NULL, m_bShowIDMap))
                m_bShowIDMap = !m_bShowIDMap;

            if (ImGui::MenuItem("Outliner", NULL, m_bShowOutliner))
                m_bShowOutliner = !m_bShowOutliner;

            if (ImGui::MenuItem("ContentBrowser", NULL, m_bShowContentBrowser))
                m_bShowContentBrowser = !m_bShowContentBrowser;

            if (ImGui::MenuItem("Toolbar", NULL, m_bShowToolbar))
                m_bShowToolbar = !m_bShowToolbar;

            if (ImGui::MenuItem("Assets", NULL, m_bShowAssets))
                m_bShowAssets = !m_bShowAssets;

            if (ImGui::MenuItem("Output Log", NULL, m_bShowOutputLog))
                m_bShowOutputLog = !m_bShowOutputLog;

            if (ImGui::MenuItem("Physics2D Manager", NULL, m_bShowPhysics2DMgr))
                m_bShowPhysics2DMgr = !m_bShowPhysics2DMgr;

            if (ImGui::MenuItem("Physics Manager", NULL, m_bShowPhysicsMgr))
                m_bShowPhysicsMgr = !m_bShowPhysicsMgr;

            if (ImGui::MenuItem("Tags & Layers", NULL, m_bShowTagsAndLayers))
                m_bShowTagsAndLayers = !m_bShowTagsAndLayers;

            if (ImGui::MenuItem("Multi Render Target", NULL, m_bShowMRT))
                m_bShowMRT = !m_bShowMRT;

            if (ImGui::MenuItem("SSAO", NULL, m_bShowSSAO))
                m_bShowSSAO = !m_bShowSSAO;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets"))
        {
            if (ImGui::MenuItem("Create Material"))
                m_ModalAssetType = ASSET_TYPE::MATERIAL;

            if (ImGui::MenuItem("Create Physics2D Material"))
                m_ModalAssetType = ASSET_TYPE::PHYSICS2D_MATERIAL;

            if (ImGui::MenuItem("Create Physic Material"))
                m_ModalAssetType = ASSET_TYPE::PHYSIC_MATERIAL;

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // ======================
    // Create Asset Popup
    // ======================
    if (ASSET_TYPE::END != m_ModalAssetType)
    {
        string popupID = ASSET_TYPE_STRING[(UINT)m_ModalAssetType];
        popupID += "##Create Asset";
        ImGui::OpenPopup(popupID.c_str());
    }

    CreateAssetModal();
}

void CLevelEditor::render_WorldSettings()
{
    ImGui_SetWindowClass(GetEditorType());
    ImGui::Begin("World Settings", &m_bShowWorldSettings);
    ImGui::Text("FPS : %d", CTimeMgr::GetInst()->GetFPS());
    ImGui::Text("Delta Time : %.5f", DT_ENGINE);

    bool bDebugRender = CRenderMgr::GetInst()->IsShowDebugRender();
    if (ImGui::Checkbox("Show DebugRender", &bDebugRender))
        CRenderMgr::GetInst()->SetShowDebugRender(bDebugRender);

    ImGui::Checkbox("Enable Gizmo", &m_bEnableGizmo);
    ImGui::Checkbox("Render Outline", (bool*)&g_Global.g_RenderOutline);
    ImGui::Checkbox("Draw WireFrame", (bool*)&g_Global.g_DrawAsWireFrame);

    bool bEnableBloom = CRenderMgr::GetInst()->IsEnableBloom();
    if (ImGui::Checkbox("Enable Bloom", &bEnableBloom))
        CRenderMgr::GetInst()->SetEnableBloom(bEnableBloom);

    ImGui::Checkbox("Enable SSAO", (bool*)&g_Global.g_EnableSSAO);

    ImGui::End();
}

void CLevelEditor::render_Toolbar()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    ImGui_SetWindowClass(GetEditorType());
    ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImVec4 tintColor = ImVec4(1, 1, 1, 1.f);
    ImVec4 BGColor = ImVec4(0, 0, 0, 0.f);
    float size = ImGui::GetWindowHeight() - 4.0f;

    // Translate, Rotate, Scale
    ImVec4 SelectedColor = ImVec4(0.67f, 0.46f, 0.18f, 1.f);
    if (ImGui::ImageButton((void*)m_TranslateButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor,
                           ImGuizmo::OPERATION::TRANSLATE == m_GizmoType ? SelectedColor : tintColor))
    {
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
    }
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)m_RotateButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor,
                           ImGuizmo::OPERATION::ROTATE == m_GizmoType ? SelectedColor : tintColor))
    {
        m_GizmoType = ImGuizmo::OPERATION::ROTATE;
    }
    ImGui::SameLine();
    if (ImGui::ImageButton((void*)m_ScaleButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor,
                           ImGuizmo::OPERATION::SCALE == m_GizmoType ? SelectedColor : tintColor))
    {
        m_GizmoType = ImGuizmo::OPERATION::SCALE;
    }
    ImGui::SameLine();

    // Play, Simulate, Stop
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    if (LEVEL_STATE::PLAY == pCurLevel->GetState() || LEVEL_STATE::SIMULATE == pCurLevel->GetState())
    {
        if (ImGui::ImageButton((void*)m_PauseButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor, tintColor))
        {
            GamePlayStatic::ChangeLevelState(pCurLevel, LEVEL_STATE::PAUSE);
        }

        ImGui::SameLine();

        if (ImGui::ImageButton((void*)m_StopButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor, tintColor))
        {
            GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(pCurLevel->GetName()), LEVEL_STATE::STOP);
        }
    }
    else if (LEVEL_STATE::PAUSE == pCurLevel->GetState())
    {
        if (CLevelMgr::GetInst()->GetCurrentLevel()->GetPrevState() == LEVEL_STATE::PLAY)
        {
            if (ImGui::ImageButton((void*)m_PlayButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor, tintColor))
            {
                CLevelSaveLoad::SaveLevel(pCurLevel, pCurLevel->GetName());
                GamePlayStatic::ChangeLevelState(pCurLevel, LEVEL_STATE::PLAY);
            }
        }
        else
        {
            if (ImGui::ImageButton((void*)m_SimulateButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor, tintColor))
            {
                CLevelSaveLoad::SaveLevel(pCurLevel, pCurLevel->GetName());
                GamePlayStatic::ChangeLevelState(pCurLevel, LEVEL_STATE::SIMULATE);
            }
        }

        ImGui::SameLine();

        if (ImGui::ImageButton((void*)m_StepButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor, tintColor))
        {
            pCurLevel->Step();
        }

        ImGui::SameLine();

        if (ImGui::ImageButton((void*)m_StopButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor, tintColor))
        {
            GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(pCurLevel->GetName()), LEVEL_STATE::STOP);
        }
    }
    else if (LEVEL_STATE::STOP == pCurLevel->GetState())
    {
        if (ImGui::ImageButton((void*)m_PlayButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor, tintColor))
        {
            CLevelSaveLoad::SaveLevel(pCurLevel, pCurLevel->GetName());
            GamePlayStatic::ChangeLevelState(pCurLevel, LEVEL_STATE::PLAY);
        }

        ImGui::SameLine();

        if (ImGui::ImageButton((void*)m_SimulateButtonTex->GetSRV().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, BGColor, tintColor))
        {
            CLevelSaveLoad::SaveLevel(pCurLevel, pCurLevel->GetName());
            GamePlayStatic::ChangeLevelState(pCurLevel, LEVEL_STATE::SIMULATE);
        }
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();
}

void CLevelEditor::render_Assets()
{
    ImGui_SetWindowClass(GetEditorType());
    ImGui::Begin("Assets", &m_bShowAssets);

    static ImGuiTextFilter filter;
    filter.Draw(ImGui_LabelPrefix("Filter").c_str());
    ImGui::Separator();

    for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
    {
        const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetMapAsset((ASSET_TYPE)i);

        if (ImGui::TreeNodeEx(ASSET_TYPE_STRING[i], ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                                                        ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
        {
            for (const auto& iter : mapAsset)
            {
                std::filesystem::path name = ToString(iter.second->GetName());
                string StringName = name.filename().string();
                string key = ToString(iter.first);

                if (!filter.PassFilter(StringName.c_str()))
                    continue;

                ImGui::TreeNodeEx(StringName.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);

                // Drag & Drop
                if (ImGui::BeginDragDropSource())
                {
                    ImGui::Text("%s", key.c_str(), key.size());

                    ImGui::SetDragDropPayload("LEVEL_EDITOR_ASSETS", key.c_str(), key.size());
                    ImGui::EndDragDropSource();
                }

                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
                {
                    switch ((ASSET_TYPE)i)
                    {
                    case ASSET_TYPE::MESH:
                        break;
                    case ASSET_TYPE::MESHDATA: {
                        Ptr<CMeshData> pMeshData = dynamic_cast<CMeshData*>(iter.second.Get());
                        ShowEditor(EDITOR_TYPE::MODEL, true);
                        CEditorMgr::GetInst()->GetModelEditor()->SetModel(pMeshData);
                    }
                    break;
                    case ASSET_TYPE::PREFAB:
                        break;
                    case ASSET_TYPE::TEXTURE:
                        break;
                    case ASSET_TYPE::COMPUTE_SHADER:
                        break;
                    case ASSET_TYPE::GRAPHICS_SHADER:
                        break;
                    case ASSET_TYPE::MATERIAL: {
                        Ptr<CMaterial> pMtrl = dynamic_cast<CMaterial*>(iter.second.Get());
                        ShowEditor(EDITOR_TYPE::MATERIAL, true);
                        CEditorMgr::GetInst()->GetMaterialEditor()->SetMaterial(pMtrl);
                    }
                    break;
                    case ASSET_TYPE::SOUND: {
                        Ptr<CSound> pSound = dynamic_cast<CSound*>(iter.second.Get());
                        ShowEditor(EDITOR_TYPE::SOUND, true);
                        CEditorMgr::GetInst()->GetSoundEditor()->SetSound(pSound);
                    }
                    break;
                    case ASSET_TYPE::PHYSICS2D_MATERIAL: {
                        Ptr<CPhysics2DMaterial> pMtrl = dynamic_cast<CPhysics2DMaterial*>(iter.second.Get());
                        ShowEditor(EDITOR_TYPE::PHYSICS2D_MATERIAL, true);
                        CEditorMgr::GetInst()->GetPhysics2DMaterialEditor()->SetMaterial(pMtrl);
                    }
                    break;
                    case ASSET_TYPE::PHYSIC_MATERIAL: {
                        Ptr<CPhysicMaterial> pMtrl = dynamic_cast<CPhysicMaterial*>(iter.second.Get());
                        ShowEditor(EDITOR_TYPE::PHYSIC_MATERIAL, true);
                        CEditorMgr::GetInst()->GetPhysicMaterialEditor()->SetMaterial(pMtrl);
                    }
                    break;
                    }
                }

                // Tooltip
                if (ImGui::IsItemHovered() && ASSET_TYPE::TEXTURE == (ASSET_TYPE)i)
                {
                    if (ImGui::BeginItemTooltip())
                    {
                        Ptr<CTexture> HoveredTex = (CTexture*)iter.second.Get();
                        ImGui::Text(ToString(HoveredTex->GetName()).c_str());
                        ImGui::Text("Texture Width : %d", HoveredTex->GetWidth());
                        ImGui::Text("Texture Height : %d", HoveredTex->GetHeight());
                        ImGui::Image(((CTexture*)HoveredTex.Get())->GetSRV().Get(), ImVec2(100, 100));
                        ImGui::EndTooltip();
                    }
                }
            }

            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void CLevelEditor::CreateAssetModal()
{
    if (ASSET_TYPE::END == m_ModalAssetType)
        return;

    string popupID = ASSET_TYPE_STRING[(UINT)m_ModalAssetType];
    popupID += "##Create Asset";

    ImGui::SetNextWindowSize(ImVec2(17.f * popupID.size(), 125.f));
    if (ImGui::BeginPopupModal(popupID.c_str(), NULL, ImGuiWindowFlags_NoResize))
    {
        static char buffer[256];
        string InputTextStr = ASSET_TYPE_STRING[(UINT)m_ModalAssetType];
        InputTextStr += " Name";
        ImGui::InputText(ImGui_LabelPrefix(InputTextStr.c_str(), 0.65f).c_str(), buffer, sizeof(buffer));

        if (ImGui::Button("Create", ImVec2(120, 0)))
        {
            switch (m_ModalAssetType)
            {
            case ASSET_TYPE::MATERIAL: {
                Ptr<CMaterial> pMtrl = new CMaterial(false);
                wstring name = L"material\\" + ToWstring(buffer) + L".mtrl";
                pMtrl->SetName(name);
                CAssetMgr::GetInst()->AddAsset(name, pMtrl);
                pMtrl->Save(name);
            }
            break;
            case ASSET_TYPE::PHYSICS2D_MATERIAL: {
                Ptr<CPhysics2DMaterial> pMtrl = new CPhysics2DMaterial(false);
                wstring name = L"physics2Dmaterial\\" + ToWstring(buffer) + L".physic2Dmtrl";
                pMtrl->SetName(name);
                CAssetMgr::GetInst()->AddAsset(name, pMtrl);
                pMtrl->Save(name);
            }
            break;
            case ASSET_TYPE::PHYSIC_MATERIAL: {
                Ptr<CPhysicMaterial> pMtrl = new CPhysicMaterial(false);
                wstring name = L"physicMaterial\\" + ToWstring(buffer) + L".physicMaterial";
                pMtrl->SetName(name);
                CAssetMgr::GetInst()->AddAsset(name, pMtrl);
                pMtrl->Save(name);
            }
            break;
            }

            memset(buffer, 0, sizeof(buffer));
            m_ModalAssetType = ASSET_TYPE::END;
        }

        ImGui::SetItemDefaultFocus();

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            memset(buffer, 0, sizeof(buffer));
            m_ModalAssetType = ASSET_TYPE::END;
        }

        ImGui::EndPopup();
    }
}

void CLevelEditor::render_Viewport()
{
    ImGui_SetWindowClass(GetEditorType());
    ImGui::Begin("Level ViewPort##CLevelEditor");

    // RT Copy
    CRenderMgr::GetInst()->CopyRTTexToRTCopyTex();

    // Viewport에서의 마우스 위치 등록
    ImVec2 viewportPos = ImGui::GetCursorScreenPos();
    CEditorMgr::GetInst()->SetViewportMousePos(Vec2(ImGui::GetIO().MousePos.x - viewportPos.x, ImGui::GetIO().MousePos.y - viewportPos.y));

    // 상태 확인
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    // 크기 등록
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    CEditorMgr::GetInst()->SetViewportSize(Vec2(viewportSize.x, viewportSize.y));

    // 렌더링
    Ptr<CTexture> pCopyTex = CRenderMgr::GetInst()->GetRTCopyTex();
    ImGui::Image((void*)pCopyTex->GetSRV().Get(), viewportSize);

    // ImGuizmo
    if (m_bEnableGizmo)
        render_ImGuizmo();

    // Drag & Drop
    if (ImGui::BeginDragDropTarget())
    {
        // Content Browser
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            string name = (char*)payload->Data;
            name.resize(payload->DataSize);
            std::filesystem::path fileNameStr = name;

            // Level 불러오기
            if (fileNameStr.extension() == CLevelSaveLoad::GetLevelExtension())
            {
                CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(fileNameStr.filename().wstring());

                if (nullptr != pLoadedLevel)
                    GamePlayStatic::ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);
            }
        }

        // Assets
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_ASSETS"))
        {
            string AssetStr = (char*)payload->Data;
            AssetStr.resize(payload->DataSize);
            std::filesystem::path AssetPath = AssetStr;

            CGameObject* pObj = nullptr;

            // Prefab
            if (L".pref" == AssetPath.extension())
            {
                Ptr<CPrefab> pPrefab = CAssetMgr::GetInst()->Load<CPrefab>(AssetPath, AssetPath);
                pObj = pPrefab->Instantiate();
            }
            else if (L".mdat" == AssetPath.extension())
            {
                Ptr<CMeshData> pMeshData = CAssetMgr::GetInst()->Load<CMeshData>(AssetPath, AssetPath);
                pObj = pMeshData->Instantiate();
            }

            if (nullptr != pObj)
            {
                // 카메라위치 기준 생성
                CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();
                Vec3 pos = pCam->Transform()->GetWorldPos();
                Vec3 dir = pCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
                pos += dir.Normalize() * 5.f;
                pObj->Transform()->SetLocalPos(pos);

                GamePlayStatic::SpawnGameObject(pObj, pObj->GetLayerIdx());
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

void CLevelEditor::render_ImGuizmo()
{
    CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();
    if (nullptr == pSelectedObj || -1 == pSelectedObj->GetLayerIdx())
        return;

    CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();

    if (nullptr == pCam)
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
        snapValue = 10.f;
    else if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
        snapValue = 15.f;
    else if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
        snapValue = 1.f;

    float snapValues[3] = {snapValue, snapValue, snapValue};

    ImGuizmo::Manipulate(*CamView.m, *CamProj.m, m_GizmoType, ImGuizmo::LOCAL, *WorldMat.m, nullptr, snap ? snapValues : nullptr);

    if (ImGuizmo::IsUsing())
    {
        Matrix originWorldMat = pTr->GetWorldMat();

        // 부모 오브젝트가 존재한 경우 부모행렬을 상쇄
        if (nullptr != pTr->GetOwner()->GetParent())
        {
            Matrix InvTransformationMat = pTr->GetTransformationMat().Invert();

            WorldMat *= InvTransformationMat;
            originWorldMat *= InvTransformationMat;
        }

        // ImGuizmo변화량이 적용된 Matrix와 원본 Matrix SRT 분해
        Vec3 Translation, Rotation, Scale;
        ImGuizmo::DecomposeMatrixToComponents(*WorldMat.m, Translation, Rotation, Scale);

        Vec3 OriginTranslation, OriginRotation, OriginScale;
        ImGuizmo::DecomposeMatrixToComponents(*originWorldMat.m, OriginTranslation, OriginRotation, OriginScale);

        // ImGuizmo로 조정한 변화량 추출
        Vec3 vPosOffset = OriginTranslation - Translation;
        Vec3 vRotOffset = OriginRotation - Rotation;
        vRotOffset.ToRadian();
        Vec3 vScaleOffset = OriginScale - Scale;

        // 부모 ↔ 자식 계층 구조이기 때문에 변화량을 계산해서 적용
        pTr->SetLocalPos(pTr->GetLocalPos() - vPosOffset);
        pTr->SetLocalRotation(pTr->GetLocalRotation() - vRotOffset);
        pTr->SetLocalScale(pTr->GetLocalScale() - vScaleOffset);
    }
}

void CLevelEditor::render_Physics2DMgr()
{
    ImGui_SetWindowClass(GetEditorType());
    if (!ImGui::Begin("Physics2D Manager##CLevelEditor", &m_bShowPhysics2DMgr))
    {
        ImGui::End();
        return;
    }

    Vec2 Gravity = CPhysics2DMgr::GetInst()->GetGravity();
    if (ImGui::DragFloat2(ImGui_LabelPrefix("Gravity").c_str(), &Gravity.x, 0.01f))
        CPhysics2DMgr::GetInst()->SetGravity(Gravity);

    float PPM = CPhysics2DMgr::GetInst()->GetPPM();
    if (ImGui::DragFloat(ImGui_LabelPrefix("Pixel Per Meter").c_str(), &PPM, 1.f, 0.0f, D3D11_FLOAT32_MAX))
        CPhysics2DMgr::GetInst()->SetPPM(PPM);

    ImGui::Separator();

    if (ImGui::Button("All Layer Enable"))
        CPhysics2DMgr::GetInst()->EnableAllLayer();

    ImGui::SameLine();

    if (ImGui::Button("All Layer Disable"))
        CPhysics2DMgr::GetInst()->DisableAllLayer();

    string column_names[LAYER_MAX + 1] = {"Layers"};
    for (int i = 1; i <= LAYER_MAX; i++)
    {
        const wstring& LayerName = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i - 1)->GetName();
        column_names[i] = ToString(LayerName);
    }

    const int columns_count = LAYER_MAX + 1; // columns 0 is Layer Name

    static bool bools[columns_count * LAYER_MAX] = {}; // Dummy storage selection storage
    for (UINT iRow = 0; iRow < LAYER_MAX; ++iRow)
    {
        for (UINT iCol = iRow; iCol < LAYER_MAX; ++iCol)
        {
            if (CPhysics2DMgr::GetInst()->GetCollisionLayer(iRow) & (1 << iCol))
            {
                bools[iRow * columns_count + iCol + 1] = true;
            }
            else
            {
                bools[iRow * columns_count + iCol + 1] = false;
            }
        }
    }

    static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
                                         ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable |
                                         ImGuiTableFlags_HighlightHoveredColumn;

    if (ImGui::BeginTable("Collision Matrix##Physics2DMgr", columns_count, table_flags, ImGui::GetContentRegionAvail()))
    {
        ImGui::TableSetupColumn(column_names[0].c_str(), ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
        for (int n = columns_count - 1; n >= 1; n--)
            ImGui::TableSetupColumn(column_names[n].c_str(), ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed);

        ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the
                                        // ImGuiTableColumnFlags_AngledHeader flag.
        ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other   functions.

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
                        CPhysics2DMgr::GetInst()->LayerCheck(row, column - 1, bools[row * columns_count + column]);
                    ImGui::PopID();
                }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

void CLevelEditor::render_PhysicsMgr()
{
    ImGui_SetWindowClass(GetEditorType());
    if (!ImGui::Begin("Physics Manager##CLevelEditor", &m_bShowPhysicsMgr))
    {
        ImGui::End();
        return;
    }

    Vec3 Gravity = CPhysicsMgr::GetInst()->GetGravity();
    if (ImGui::DragFloat3(ImGui_LabelPrefix("Gravity").c_str(), &Gravity.x, 0.01f))
        CPhysicsMgr::GetInst()->SetGravity(Gravity);

    float PPM = CPhysicsMgr::GetInst()->GetPPM();
    if (ImGui::DragFloat(ImGui_LabelPrefix("Pixel Per Meter").c_str(), &PPM, 1.f, 0.0f, D3D11_FLOAT32_MAX))
        CPhysicsMgr::GetInst()->SetPPM(PPM);

    ImGui::Separator();

    if (ImGui::Button("All Layer Enable"))
        CPhysicsMgr::GetInst()->EnableAllLayer();

    ImGui::SameLine();

    if (ImGui::Button("All Layer Disable"))
        CPhysicsMgr::GetInst()->DisableAllLayer();

    string column_names[LAYER_MAX + 1] = {"Layers"};
    for (int i = 1; i <= LAYER_MAX; i++)
    {
        const wstring& LayerName = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i - 1)->GetName();
        column_names[i] = ToString(LayerName);
    }

    const int columns_count = LAYER_MAX + 1; // columns 0 is Layer Name

    static bool bools[columns_count * LAYER_MAX] = {}; // Dummy storage selection storage
    for (UINT iRow = 0; iRow < LAYER_MAX; ++iRow)
    {
        for (UINT iCol = iRow; iCol < LAYER_MAX; ++iCol)
        {
            if (CPhysicsMgr::GetInst()->GetCollisionLayer(iRow) & (1 << iCol))
            {
                bools[iRow * columns_count + iCol + 1] = true;
            }
            else
            {
                bools[iRow * columns_count + iCol + 1] = false;
            }
        }
    }

    static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
                                         ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable |
                                         ImGuiTableFlags_HighlightHoveredColumn;

    if (ImGui::BeginTable("Collision Matrix##Physics2DMgr", columns_count, table_flags, ImGui::GetContentRegionAvail()))
    {
        ImGui::TableSetupColumn(column_names[0].c_str(), ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
        for (int n = columns_count - 1; n >= 1; n--)
            ImGui::TableSetupColumn(column_names[n].c_str(), ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed);

        ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the
                                        // ImGuiTableColumnFlags_AngledHeader flag.
        ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other   functions.

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
                        CPhysicsMgr::GetInst()->LayerCheck(row, column - 1, bools[row * columns_count + column]);
                    ImGui::PopID();
                }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

void CLevelEditor::render_TagsAndLayers()
{
    ImGui_SetWindowClass(GetEditorType());
    if (!ImGui::Begin("Tags & Layers##CLevelEditor", &m_bShowTagsAndLayers))
    {
        ImGui::End();
        return;
    }

    // Tags
    if (ImGui::TreeNodeEx("Tags##LevelEditor", 0, "Tags"))
    {

        ImGui::TreePop();
    }

    // Layers
    if (ImGui::TreeNodeEx("Layers##LevelEditor", 0, "Layers"))
    {
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        for (UINT i = 0; i < LAYER_MAX; i++)
        {
            CLayer* pCurLayer = pCurLevel->GetLayer(i);

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            string LayerName = ToString(pCurLayer->GetName());
            strcpy_s(buffer, sizeof(buffer), LayerName.c_str());

            string label = "User Layer ";
            label += std::to_string(i);
            if (ImGui::InputText(ImGui_LabelPrefix(label.c_str()).c_str(), buffer, sizeof(buffer)))
            {
                pCurLayer->SetName(ToWstring(buffer));
            }
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

void CLevelEditor::render_MRT()
{
    ImGui_SetWindowClass(GetEditorType());
    if (!ImGui::Begin("Multi Render Target##CLevelEditor", &m_bShowMRT))
    {
        ImGui::End();
        return;
    }

    CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED);

    for (UINT i = 0; i < 8; i++)
    {
        Ptr<CTexture> pTex = pMRT->GetRenderTargetTex(i);
        if (nullptr == pTex)
            continue;

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 250);
        ImGui::Spacing();
        ImGui::Dummy(ImVec2(0.f, 75.f));
        ImGui::Text(ToString(pTex->GetKey()).c_str());
        ImGui::NextColumn();

        ImGui::Image((void*)pTex->GetSRV().Get(), ImVec2(200.f, 200.f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));

        ImGui::Columns(1);
    }

    Ptr<CTexture> pDSTex = pMRT->GetDepthStencilTex();
    if (nullptr != pDSTex)
    {
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 250);
        ImGui::Spacing();
        ImGui::Dummy(ImVec2(0.f, 75.f));
        ImGui::Text(ToString(pDSTex->GetKey()).c_str());
        ImGui::NextColumn();

        ImGui::Image((void*)pDSTex->GetSRV().Get(), ImVec2(200.f, 200.f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));

        ImGui::Columns(1);
    }

    Ptr<CTexture> pLightTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"LightRadianceTargetTex");
    if (nullptr != pLightTex)
    {
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 250);
        ImGui::Spacing();
        ImGui::Dummy(ImVec2(0.f, 75.f));
        ImGui::Text(ToString(pLightTex->GetKey()).c_str());
        ImGui::NextColumn();

        ImGui::Image((void*)pLightTex->GetSRV().Get(), ImVec2(200.f, 200.f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));

        ImGui::Columns(1);
    }

    ImGui::End();
}

void CLevelEditor::render_SSAO()
{
    ImGui_SetWindowClass(GetEditorType());
    if (!ImGui::Begin("SSAO##CLevelEditor", &m_bShowSSAO))
    {
        ImGui::End();
        return;
    }

    CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SSAO);

    Ptr<CTexture> pTex = pMRT->GetRenderTargetTex(0);
    if (nullptr != pTex)
    {
        ImGui::Image((void*)pTex->GetSRV().Get(), ImGui::GetContentRegionAvail());
    }

    ImGui::End();
}

void CLevelEditor::render_CamPreview()
{
    CGameObject* SelectedObj = CEditorMgr::GetInst()->GetSelectedObject();
    if (nullptr == SelectedObj || nullptr == SelectedObj->Camera() || CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
        return;

    ImGui_SetWindowClass(GetEditorType());
    string Name = ToString(SelectedObj->GetName());
    Name += "##CLevelEditor";
    ImGui::Begin(Name.c_str(), NULL, ImGuiWindowFlags_NoDocking);

    Ptr<CTexture> pCameraPreviewTex = CRenderMgr::GetInst()->GetCameraPreviewTex();
    ImGui::Image((void*)pCameraPreviewTex->GetSRV().Get(), ImGui::GetContentRegionAvail());

    ImGui::End();
}
