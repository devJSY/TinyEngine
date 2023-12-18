#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel : public CEntity
{
private:
    CLayer* m_arrLayer[LAYER_MAX];

public:
    void begin();
    void tick();
    void finaltick();
    void render();

    void AddObject(CGameObject* _Object, int _LayerIdx);

public:
    CLevel();
    virtual ~CLevel();
};