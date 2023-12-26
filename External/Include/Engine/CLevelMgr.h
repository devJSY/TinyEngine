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
    CLevel* GetCurrentLevel() const { return m_CurLevel; }

private:
    CGameObject* m_CamObj;
    CGameObject* m_SelectedObj;

public:
    void SetCameraObj(CGameObject* cam) { m_CamObj = cam; }
    CGameObject* GetCameraObj() const { return m_CamObj; }

public:
    CGameObject* GetSelectedObj() const { return m_SelectedObj; }
    void SetSelectObj(CGameObject* obj) { m_SelectedObj = obj; }

public:
    void init();
    void tick();
    void render();
};
