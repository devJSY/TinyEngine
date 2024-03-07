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

    bool m_bShowWorldSettings;
    bool m_bShowViewport;
    bool m_bShowIDMap;
    bool m_bShowOutliner;
    bool m_bShowContentBrowser;
    bool m_bShowToolbar;
    bool m_bShowAssets;
    bool m_bShowOutputLog;

    bool m_bShowMaterialEditor;
    bool m_bShowBlueprintEditor;
    bool m_bShowSpriteEditor;
    bool m_bShowTileMapEditor;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render() override;

private:
    void render_MenuBar();
    void render_WorldSettings();
    void render_Viewport();
    void render_ImGuizmo();
    void render_CollisionResponses();
    void render_Toolbar();
    void render_Assets();

public:
    bool IsViewportFocused() const { return m_ViewportFocused; }
    bool IsViewportHovered() const { return m_ViewportHovered; }

public:
    void ShowMaterialEditor(bool _show) { m_bShowMaterialEditor = _show; }
    void ShowBlueprintEditor(bool _show) { m_bShowBlueprintEditor = _show; }
    void ShowSpriteEditor(bool _show) { m_bShowSpriteEditor = _show; }
    void ShowTileMapEditor(bool _show) { m_bShowTileMapEditor = _show; }

public:
    CLevelEditor();
    virtual ~CLevelEditor();
};
