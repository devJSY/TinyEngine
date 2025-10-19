#pragma once
#include "CEditor.h"

class CCamera;
class CGameObjectEx;

class CMaterialEditor : public CEditor
{
private:
    Ptr<CMaterial> m_Mtrl;

    // Viewport
    Ptr<CTexture> m_ViewportRTTex;
    CCamera* m_ViewportCam;
    CGameObjectEx* m_ViewportObj;
    CGameObjectEx* m_LightObj;
    CStructuredBuffer* m_LightBuffer;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render(bool* open) override;

private:
    void render();

private:
    void DrawViewport();
    void DrawDetails();

public:
    void SetMaterial(Ptr<CMaterial> _Mtrl) { m_Mtrl = _Mtrl; }

public:
    CMaterialEditor();
    virtual ~CMaterialEditor();
};
