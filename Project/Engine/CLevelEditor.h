#pragma once
#include "CEditor.h"

class CLevelEditor : public CEditor
{
private:
    // Viewport
    ComPtr<ID3D11Texture2D> m_ViewportRTTex;
    ComPtr<ID3D11ShaderResourceView> m_ViewportSRView;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render() override;
    virtual void Resize() override;

private:
    void CreateViewport();
    void SetDarkThemeColors();

public:
    CLevelEditor();
    virtual ~CLevelEditor();
};
