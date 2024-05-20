#pragma once
#pragma once
#include "CEditor.h"
#include "COutliner.h"
#include "CContentBrowser.h"
#include "Ptr.h"
#include "CTexture.h"

class CLevelEditor : public CEditor
{
private:
    bool m_ViewportFocused;
    bool m_ViewportHovered;

    COutliner m_Outliner;
    CContentBrowser m_ContentBrowser;

    ImGuizmo::OPERATION m_GizmoType;
    bool m_bEnableGizmo;

    bool m_bShowEditor[(UINT)EDITOR_TYPE::END];

    Ptr<CTexture> m_PlayButtonTex;
    Ptr<CTexture> m_SimulateButtonTex;
    Ptr<CTexture> m_StepButtonTex;
    Ptr<CTexture> m_PauseButtonTex;
    Ptr<CTexture> m_StopButtonTex;
    Ptr<CTexture> m_TranslateButtonTex;
    Ptr<CTexture> m_RotateButtonTex;
    Ptr<CTexture> m_ScaleButtonTex;

    ASSET_TYPE m_ModalAssetType;

private:
    // Windows Menu 
    bool m_bShowWorldSettings;
    bool m_bShowViewport;
    bool m_bShowIDMap;
    bool m_bShowOutliner;
    bool m_bShowContentBrowser;
    bool m_bShowToolbar;
    bool m_bShowAssets;
    bool m_bShowOutputLog;
    bool m_bShowPhysics2DMgr;
    bool m_bShowPhysicsMgr;
    bool m_bShowTagsAndLayers;
    bool m_bShowMRT;
    bool m_bShowSSAO;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render(bool* open) override{}; // Not Used
    void render();

private:
    void render_MenuBar();
    void render_WorldSettings();
    void render_Viewport();
    void render_ImGuizmo();
    void render_Toolbar();
    void render_Assets();
    void render_Physics2DMgr();
    void render_PhysicsMgr();
    void render_TagsAndLayers();
    void render_MRT();
    void render_SSAO();
    void render_MainCamPreview();

private:
    void CreateAssetModal();

public:
    bool IsViewportFocused() const { return m_ViewportFocused; }
    bool IsViewportHovered() const { return m_ViewportHovered; }

public:
    void ShowEditor(EDITOR_TYPE _Type, bool _show) { m_bShowEditor[(UINT)_Type] = _show; }

public:
    CLevelEditor();
    virtual ~CLevelEditor();
};
