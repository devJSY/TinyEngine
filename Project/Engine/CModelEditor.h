#pragma once
#include "CEditor.h"

class CGameObjectEx;
class CCamera;

class CStructuredBuffer;

class CTexture;
class CMeshData;

class CModelEditor : public CEditor
{
private:
    CGameObjectEx* m_ModelObj;
    int m_SelectedBoneIdx;
    vector<Matrix> m_FinalBoneMat;

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
    void DrawSkeletonTree();
    void SkeletonRe(const vector<tMTBone>& _vecBone, int _BoneIdx, int _NodeOpenFlag);
    void DrawAnimation();

public:
    void SetModel(Ptr<CMeshData> _MeshData);

public:
    CModelEditor();
    virtual ~CModelEditor();
};
