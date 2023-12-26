#pragma once
#include "CEntity.h"

class CLevel;
class CGameObject;

class CLevelMgr : public CSingleton<CLevelMgr>
{
    SINGLE(CLevelMgr);

private:
    CLevel* m_CurLevel;

public:
    CLevel* GetCurLevel() const { return m_CurLevel; }

private:
    CGameObject* m_CamObj;

public:
    void SetCameraObj(CGameObject* cam) { m_CamObj = cam; }
    CGameObject* GetCameraObj() const { return m_CamObj; }

public:
    void init();
    void tick();
    void render();
};
