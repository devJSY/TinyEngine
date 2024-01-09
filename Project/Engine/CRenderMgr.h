#pragma once
#include "singleton.h"
#include "Ptr.h"
#include "CTexture.h"

class CGameObject;
class CCamera;
class CLight2D;

class CStructuredBuffer;

class CRenderMgr : public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);

private:
    vector<CCamera*> m_vecCam;

    CStructuredBuffer* m_Light2DBuffer;
    vector<CLight2D*> m_vecLight2D;

    list<tDebugShapeInfo> m_DbgShapeInfo;

    CGameObject* m_pDebugObj;

    bool m_bShowDebugRender;
    bool m_bShowCollider;

public:
    void RegisterCamera(CCamera* _Cam, int _Idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }

    void SetShowDebugRender(bool _OnOff) { m_bShowDebugRender = _OnOff; }
    bool IsShowDebugRender() const { return m_bShowDebugRender; }

    void SetShowCollider(bool _OnOff) { m_bShowCollider = _OnOff; }
    bool IsShowCollider() const { return m_bShowCollider; }

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }

public:
    CCamera* GetCamera(int _Idx) const;
    void ClearCamera() { m_vecCam.clear(); }

public:
    void init();
    void tick();

public:
    void CopyRenderTarget();
    void CreateRTCopyTex(Vec2 Resolution);
    void Resize(Vec2 Resolution);

private:
    void render();
    void render_debug();

    // 리소스 바인딩
    void UpdateData();

    // 리소스 클리어
    void Clear();
};
