#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer : public CEntity
{
private:
    vector<CGameObject*> m_vecParent;
    vector<CGameObject*> m_vecObjects;

    int m_iLayerIdx;

public:
    const vector<CGameObject*>& GetParentObject() { return m_vecParent; }

public:
    void begin();
    void tick();
    void finaltick();
    void render();

public:
    // 특정 오브젝트를 레이어에서 제거
    void DetachGameObject(CGameObject* _Object);
    void RegisterGameObject(CGameObject* _Object) { m_vecObjects.push_back(_Object); }

public:
    CLayer();
    virtual ~CLayer();

private:
    void AddObject(CGameObject* _Object, bool _bMove);

    friend class CLevel;
};
