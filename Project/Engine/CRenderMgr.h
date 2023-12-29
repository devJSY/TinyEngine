#pragma once
#include "singleton.h"

class CCamera;
class CGameObject;

class CRenderMgr : public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);

private:
    vector<CCamera*> m_vecCam;

    list<tDebugShapeInfo> m_DbgShapeInfo;

    CGameObject* m_pDebugObj;

public:
    void RegisterCamera(CCamera* _Cam, int _Idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }

public:
    CCamera* GetCamera(int _Idx) const;

public:
    void init();
    void tick();

private:
    void render();
    void render_debug();
};
