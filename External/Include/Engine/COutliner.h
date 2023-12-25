#pragma once
#include "CEntity.h"

class CGameObject;

class COutliner : public CEntity
{
private:
    CGameObject* m_SelectedObj;

private:
    void DrawNode(CGameObject* obj);
    void DrawDetails(CGameObject* obj);

public:
    CGameObject* GetSelectedObj() const { return m_SelectedObj; }

public:
    void begin();
    void render();

private:
    COutliner();
    ~COutliner();

    friend class CLevelEditor; // CLevelEditor 에서만 생성가능하도록 제한
};
