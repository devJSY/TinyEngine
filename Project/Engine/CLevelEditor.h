#pragma once
#include "CEditor.h"
#include "COutliner.h"
#include "CContentBrowser.h"
#include "Ptr.h"
#include "CTexture.h"

class CLevelEditor : public CEditor
{
private:
    Ptr<CTexture> m_RTCopyTex;

    bool m_ViewportFocused;
    bool m_ViewportHovered;

    bool m_show_Viewport2;

    COutliner m_Outliner;

    ImGuizmo::OPERATION m_GizmoType;

    CContentBrowser m_ContentBrowser;

    float m_FontSize;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render() override;
    virtual void Resize() override;

private:
    void SetDarkThemeColors();
    void CreateViewport();

public:
    bool IsViewportFocused() const { return m_ViewportFocused; }
    bool IsViewportHovered() const { return m_ViewportHovered; }

public:
    CLevelEditor();
    virtual ~CLevelEditor();
};
