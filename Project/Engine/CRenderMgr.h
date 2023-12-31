#pragma once
#include "singleton.h"
#include "Ptr.h"
#include "CTexture.h"

class CCamera;
class CGameObject;

class CRenderMgr : public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);

private:
    vector<CCamera*> m_vecCam;

    list<tDebugShapeInfo> m_DbgShapeInfo;

    CGameObject* m_pDebugObj;

private:
    Ptr<CTexture> m_RTCopyTex;

public:
    void RegisterCamera(CCamera* _Cam, int _Idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }

public:
    CCamera* GetCamera(int _Idx) const;

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
};
