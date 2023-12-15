#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer : public CEntity
{
private:
    vector<CGameObject*> m_vecParents;

public:
    void begin();
    void tick();
    void finaltick();
    void render();

    void AddObject(CGameObject* _Object) { m_vecParents.push_back(_Object); }

public:
    CLayer();
    ~CLayer();
};
