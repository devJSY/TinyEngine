#pragma once
#include "CEntity.h"
#include "CLayer.h"

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
    CLayer* GetLayer(UINT _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }

public:
    CLevel();
    virtual ~CLevel();
};