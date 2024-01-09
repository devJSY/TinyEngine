#pragma once
#pragma once
#include "CEntity.h"

class CGameObject;

class COutliner : public CEntity
{
private:
    ImGuiTreeNodeFlags m_DefaultTreeNodeFlag;

private:
    void DrawNode(CGameObject* obj);
    void DrawDetails(CGameObject* obj);

public:
    void init();
    void render();

private:
    void DrawTransform(CGameObject* obj);
    void DrawCollider2D(CGameObject* obj);
    void DrawCollider3D(CGameObject* obj);
    void DrawAnimator2D(CGameObject* obj);
    void DrawAnimator3D(CGameObject* obj);
    void DrawLight2D(CGameObject* obj);
    void DrawLight3D(CGameObject* obj);
    void DrawCamera(CGameObject* obj);
    void DrawMeshRender(CGameObject* obj);
    void DrawTileMap(CGameObject* obj);
    void DrawParticlesystem(CGameObject* obj);
    void DrawSkybox(CGameObject* obj);
    void DrawLandscape(CGameObject* obj);

private:
    COutliner();
    virtual ~COutliner();

    friend class CLevelEditor; // CLevelEditor 에서만 생성가능하도록 제한
};
