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
    CGameObject* m_SelectedPreviewObj;

    bool m_bDrawWireFrame;
    bool m_bMeshSaved;

    // Viewport
    vector<tInstObj> m_vecDeferred;
    vector<tInstObj> m_vecForward;

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
    void SortObject();
    void render_ShadowMap();
    void render_ImGizmo();

    void DrawDetails();
    void DrawSkeletonTree();
    void DrawBoneSocket(tMTBone& _Bone);
    void SkeletonRe(vector<tMTBone>& _vecBone, int _BoneIdx, int _NodeOpenFlag);
    void DrawAnimation(CGameObject* _Obj, const string& _TreeNodeName);
    void finaltick_ModelEditor(CGameObject* _Obj);

public:
    void Resize(Vec2 resolution);
    void SetModel(Ptr<CMeshData> _MeshData);

    bool IsViewportFocused() const { return m_ViewportFocused; }
    bool IsViewportHovered() const { return m_ViewportHovered; }

public:
    CModelEditor();
    virtual ~CModelEditor();
};
