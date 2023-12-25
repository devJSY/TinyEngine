#pragma once
#include "CEditor.h"
#include "COutliner.h"

class CLevelEditor : public CEditor
{
private:
    // Viewport
    ComPtr<ID3D11Texture2D> m_ViewportRTTex;
    ComPtr<ID3D11ShaderResourceView> m_ViewportSRView;

    bool m_ViewportFocused;
    bool m_ViewportHovered;

    bool m_show_Viewport2;

    COutliner m_Outliner;

    ImGuizmo::OPERATION m_GizmoType;

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
