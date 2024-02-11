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
    vector<CCamera*> m_vecCam;
    CCamera* m_CamUI;

    Ptr<CTexture> m_RTCopyTex;
    Ptr<CTexture> m_IDMapTex;
    Ptr<CTexture> m_IDMapDSTex;
    Ptr<CTexture> m_PostProcessTex;
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

    // Post Process
    vector<CGameObject*> m_vecPostProcess;
    int bloomLevels;
    vector<Ptr<CTexture>> m_BloomTextures;
    vector<CGameObject*> m_BloomDownFilters;
    vector<CGameObject*> m_BloomUpFilters;
    CGameObject* m_ToneMappingObj;

    // Mirror
    CGameObject* m_Mirror;

public:
    void RegisterCamera(CCamera* _Cam, int _Idx);
    void RegisterUICamera(CCamera* _Cam);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }

    void SetShowDebugRender(bool _OnOff) { m_bShowDebugRender = _OnOff; }
    bool IsShowDebugRender() const { return m_bShowDebugRender; }

    void SetShowCollider(bool _OnOff) { m_bShowCollider = _OnOff; }
    bool IsShowCollider() const { return m_bShowCollider; }

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }
    void RegisterLight3D(CLight3D* _Light3D) { m_vecLight3D.push_back(_Light3D); }

    void RegisterMirror(CGameObject* _mirror) { m_Mirror = _mirror; }

    void RegisterPostProcess(CGameObject* _PostProcessObj) { m_vecPostProcess.push_back(_PostProcessObj); }

public:
    CCamera* GetCamera(int _Idx) const;
    CCamera* GetUICamera() const { return m_CamUI; }
    void ClearCamera()
    {
        m_vecCam.clear();
        m_CamUI = nullptr;
    }

public:
    void init();
    void tick();

public:
    void CopyRTTexToRTCopyTex();
    void CopyToPostProcessTex();
    void Resize(Vec2 Resolution);

    Ptr<CTexture> GetRTCopyTex() const { return m_RTCopyTex; }
    Ptr<CTexture> GetIDMapTex() const { return m_IDMapTex; }
    Ptr<CTexture> GetIDMapDSTex() const { return m_IDMapDSTex; }
    Ptr<CTexture> GetPostProcessTex() const { return m_PostProcessTex; }
    Ptr<CTexture> GetFloatTex() const { return m_FloatRTTex; }

private:
    void CreateRTCopyTex(Vec2 Resolution);
    void CreateIDMapTex(Vec2 Resolution);
    void CreatePostProcessTex(Vec2 Resolution);
    void CreateBloomTextures(Vec2 Resolution);

private:
    void render();
    void render_debug();
    void render_mirror();
    void render_postprocess();
    void render_ui();

    // 리소스 바인딩
    void UpdateData();

    // 리소스 클리어
    void Clear();
};
