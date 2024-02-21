#pragma once
#include "singleton.h"
#include "Ptr.h"
#include "CTexture.h"

class CGameObject;
class CCamera;
class CLight2D;
class CLight3D;

class CStructuredBuffer;

class CRenderMgr : public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);

private:
    CCamera* m_mainCam;
    vector<CCamera*> m_vecCam;
    CCamera* m_EditorCam;

    Ptr<CTexture> m_RTCopyTex;
    Ptr<CTexture> m_IDMapTex;
    Ptr<CTexture> m_IDMapDSTex;
    Ptr<CTexture> m_PostProcessTex_LDRI;
    Ptr<CTexture> m_PostProcessTex_HDRI;
    Ptr<CTexture> m_FloatRTTex;

    CStructuredBuffer* m_Light2DBuffer;
    vector<CLight2D*> m_vecLight2D;

    CStructuredBuffer* m_Light3DBuffer;
    vector<CLight3D*> m_vecLight3D;

    list<tDebugShapeInfo> m_DbgShapeInfo;

    CGameObject* m_pDebugObj;

    bool m_bShowDebugRender;
    bool m_bShowCollider;

    // NoiseTexture
    vector<Ptr<CTexture>> m_vecNoiseTex;

    // Post Effect
    Ptr<CTexture> m_DepthOnlyTex;
    CGameObject* m_PostEffectObj;

    // Post Process
    int bloomLevels;
    vector<Ptr<CTexture>> m_BloomTextures;
    vector<CGameObject*> m_BloomDownFilters;
    vector<CGameObject*> m_BloomUpFilters;
    CGameObject* m_ToneMappingObj;

    // Render function pointer
    void (CRenderMgr::*RENDER_FUNC)(void);

public:
    void RegisterCamera(CCamera* _Cam, int _Idx);
    void RegisterEditorCamera(CCamera* _Cam) { m_EditorCam = _Cam; }

    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }

    void SetShowDebugRender(bool _OnOff) { m_bShowDebugRender = _OnOff; }
    bool IsShowDebugRender() const { return m_bShowDebugRender; }

    void SetShowCollider(bool _OnOff) { m_bShowCollider = _OnOff; }
    bool IsShowCollider() const { return m_bShowCollider; }

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
    void RegisterLight3D(CLight3D* _Light3D) { m_vecLight3D.push_back(_Light3D); }

    void ActiveEditorMode(bool _bActive);

public:
    CCamera* GetMainCamera() const { return m_mainCam; };
    CCamera* GetCamera(int _Idx) const;
    void ClearCamera() { m_vecCam.clear(); }

public:
    void init();
    void render();

public:
    void Clear_Buffers(const Vec4& Color);

    void CopyRTTexToRTCopyTex();
    void CopyToPostProcessTex_HDRI();
    void CopyToPostProcessTex_LDRI();
    void Resize(Vec2 Resolution);

    Ptr<CTexture> GetRTCopyTex() const { return m_RTCopyTex; }
    Ptr<CTexture> GetIDMapTex() const { return m_IDMapTex; }
    Ptr<CTexture> GetIDMapDSTex() const { return m_IDMapDSTex; }
    Ptr<CTexture> GetPostProcessTex_LDRI() const { return m_PostProcessTex_LDRI; }
    Ptr<CTexture> GetPostProcessTex_HDRI() const { return m_PostProcessTex_HDRI; }
    Ptr<CTexture> GetDepthOnlyTex() const { return m_DepthOnlyTex; }

private:
    void CreateRTCopyTex(Vec2 Resolution);
    void CreateIDMapTex(Vec2 Resolution);
    void CreateDepthOnlyTex(Vec2 Resolution);
    void CreatePostProcessTex(Vec2 Resolution);
    void CreateBloomTextures(Vec2 Resolution);

public:
    void render_postprocess();

private:
    void render_play();
    void render_editor();

    void render_debug();
    void render_LightDepth();

    // 리소스 바인딩
    void UpdateData();

    // 리소스 클리어
    void Clear();
};
