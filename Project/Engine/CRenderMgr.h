#pragma once
#include "Ptr.h"
#include "CTexture.h"

class CGameObject;
class CCamera;
class CLight2D;
class CLight;
class CMRT;

class CStructuredBuffer;

class CRenderMgr : public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);

private:
    // MRT
    CMRT* m_arrMRT[(UINT)MRT_TYPE::END];

    // Camera
    CCamera* m_mainCam;
    vector<CCamera*> m_vecCam;
    CCamera* m_EditorCam;

    // Render Texture
    Ptr<CTexture> m_RTCopyTex;
    Ptr<CTexture> m_IDMapTex;
    Ptr<CTexture> m_IDMapDSTex;
    Ptr<CTexture> m_PostProcessTex_LDRI;
    Ptr<CTexture> m_PostProcessTex_HDRI;
    Ptr<CTexture> m_FloatRTTex;

    // Light
    CStructuredBuffer* m_Light2DBuffer;
    vector<CLight2D*> m_vecLight2D;

    CStructuredBuffer* m_LightBuffer;
    vector<CLight*> m_vecLight;

    // Debug
    list<tDebugShapeInfo> m_DbgShapeInfo;
    CGameObject* m_pDebugObj;
    bool m_bShowDebugRender;

    // NoiseTexture
    vector<Ptr<CTexture>> m_vecNoiseTex;

    // Post Effect
    Ptr<CTexture> m_DepthOnlyTex;
    CGameObject* m_PostEffectObj;

    // Post Process
    bool m_bEnableBloom;
    const UINT m_bloomLevels;

    // LDRI Bloom
    Ptr<CTexture> m_BloomRTTex_LDRI;
    vector<Ptr<CTexture>> m_BloomTextures_LDRI;
    vector<Ptr<CTexture>> m_BlurTextures;
    CGameObject* m_SamplingObj;
    CGameObject* m_BlurXObj;
    CGameObject* m_BlurYObj;
    CGameObject* m_CombineObj;

    // HDRI Bloom
    vector<Ptr<CTexture>> m_BloomTextures_HDRI;
    CGameObject* m_BloomDownObj;
    CGameObject* m_BloomUpObj;
    CGameObject* m_ToneMappingObj;

    // Depth Masking
    bool m_bEnableDepthMasking;
    Ptr<CTexture> m_DepthMaskingTex;
    CGameObject* m_DepthMaskingObj;
    UINT m_DepthMaskingLayerMask;

    // Depth of Field
    bool m_bEnableDOF;
    CGameObject* m_DOFObj;

    // Camera Preview
    Ptr<CTexture> m_CameraPreviewTex;

    // Render function pointer
    void (CRenderMgr::*RENDER_FUNC)(void);

public:
    CMRT* GetMRT(MRT_TYPE _type) const { return m_arrMRT[(UINT)_type]; }

    void RegisterCamera(CCamera* _Cam, int _Idx);
    void RegisterEditorCamera(CCamera* _Cam) { m_EditorCam = _Cam; }

    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }

    bool IsShowDebugRender() const { return m_bShowDebugRender; }
    void SetShowDebugRender(bool _OnOff) { m_bShowDebugRender = _OnOff; }

    bool IsEnableBloom() const { return m_bEnableBloom; }
    void SetEnableBloom(bool _bEnable) { m_bEnableBloom = _bEnable; }

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
    void RegisterLight(CLight* _Light) { m_vecLight.push_back(_Light); }

    void ActiveEditorMode(bool _bActive);

    bool IsEnableDepthMasking() const { return m_bEnableDepthMasking; }
    void SetEnableDepthMasking(bool _bEnable) { m_bEnableDepthMasking = _bEnable; }

    UINT GetDepthMaskingLayerMask() const { return m_DepthMaskingLayerMask; }
    void DepthMaskingLayerMask(UINT _LayerIdx, bool _bMask);

    bool IsEnableDOF() const { return m_bEnableDOF; }
    void SetEnableDOF(bool _bEnable) { m_bEnableDOF = _bEnable; }

public:
    CCamera* GetMainCamera() const { return m_mainCam; };
    CCamera* GetCamera(int _Idx) const;
    CCamera* GetEditorCamera() const { return m_EditorCam; };
    void ClearCamera()
    {
        m_mainCam = nullptr;
        m_vecCam.clear();
    }

public:
    void init();
    void render();

public:
    void CopyRTTexToRTCopyTex();
    void CopyToPostProcessTex_LDRI();
    void CopyToPostProcessTex_HDRI();

    void BlurTexture(Ptr<CTexture> _BlurTargetTex, UINT _BlurLevel, bool ApplyThreshold = false);

    void Resize_Release();
    void Resize(Vec2 Resolution);

    Ptr<CTexture> GetRTCopyTex() const { return m_RTCopyTex; }
    Ptr<CTexture> GetIDMapTex() const { return m_IDMapTex; }
    Ptr<CTexture> GetIDMapDSTex() const { return m_IDMapDSTex; }
    Ptr<CTexture> GetPostProcessTex_LDRI() const { return m_PostProcessTex_LDRI; }
    Ptr<CTexture> GetPostProcessTex_HDRI() const { return m_PostProcessTex_HDRI; }
    Ptr<CTexture> GetDepthOnlyTex() const { return m_DepthOnlyTex; }
    Ptr<CTexture> GetBloomRTTex_LDRI() const { return m_BloomRTTex_LDRI; }
    Ptr<CTexture> GetDepthMaskingTex() const { return m_DepthMaskingTex; }
    Ptr<CTexture> GetCameraPreviewTex() const { return m_CameraPreviewTex; }

    const vector<CLight2D*>& GetvecLight2D() const { return m_vecLight2D; }
    const vector<CLight*>& GetvecLight() const { return m_vecLight; }

private:
    void CreateRTCopyTex(Vec2 Resolution);
    void CreateIDMapTex(Vec2 Resolution);
    void CreateDepthOnlyTex(Vec2 Resolution);
    void CreatePostProcessTex(Vec2 Resolution);
    void CreateBloomTextures(Vec2 Resolution);
    void CreateCameraPreviewTex(Vec2 Resolution);
    void CreateMRT(Vec2 Resolution);

public:
    void render_postprocess_LDRI();
    void render_postprocess_HDRI();

public:
    void render_StaticShadowDepth();

private:
    void render_Clear(const Vec4& Color);

    void render_play();
    void render_editor();
    void render_CameraPreview();

    void render_debug();
    void render_DynamicShadowDepth();

    // 리소스 바인딩
    void UpdateData();

public:
    // 리소스 클리어
    void Clear();
};
