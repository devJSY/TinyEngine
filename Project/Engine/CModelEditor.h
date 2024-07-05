#pragma once
#include "CEditor.h"

class CGameObjectEx;
class CCamera;

class CStructuredBuffer;

class CTexture;
class CMeshData;

struct tBoneSocket;

class CModelEditor : public CEditor
{
private:
    CGameObjectEx* m_ModelObj;
    tMTBone* m_SelectedBone;
    tBoneSocket* m_SelectedBoneSocket;

    bool m_bDrawWireFrame;

    // Viewport
    Ptr<CTexture> m_ViewportRTTex;
    Ptr<CTexture> m_ViewportFloatRTTex;
    Ptr<CTexture> m_ViewportDSTex;

    CCamera* m_ViewportCam;
    CGameObjectEx* m_LightObj;
    CGameObjectEx* m_SkyBoxObj;
    CGameObjectEx* m_FloorObj;
    CGameObjectEx* m_ToneMappingObj;
    CStructuredBuffer* m_LightBuffer;

    bool m_ViewportFocused;
    bool m_ViewportHovered;
    ImGuizmo::OPERATION m_GizmoType;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual void render(bool* open) override;

private:
    void render();

private:
    void DrawViewport();
    void DrawImGizmo();
    void DrawDetails();
    void DrawSkeletonTree();
    void SkeletonRe(vector<tMTBone>& _vecBone, int _BoneIdx, int _NodeOpenFlag);
    void DrawAnimation();

public:
    void SetModel(Ptr<CMeshData> _MeshData);

    bool IsViewportFocused() const { return m_ViewportFocused; }
    bool IsViewportHovered() const { return m_ViewportHovered; }

public:
    CModelEditor();
    virtual ~CModelEditor();
};
