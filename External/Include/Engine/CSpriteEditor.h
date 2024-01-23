#pragma once
#include "CEditor.h"
#include "CAnim.h"

struct tSprite
{
    ImRect Rect;
    bool bViewport_Selected;
    bool bSpriteList_Selected;
};

class CSpriteEditor : public CEditor
{
private:
    Ptr<CTexture> m_pTex;

    ImVector<tSprite> m_Sprites;
    ImRect m_DragRect;
    ImVec2 m_ViewportOffset;
    float m_ViewportScale;

    int m_LineCheckFlag;

    // Animation Data
    vector<tAnimData2D> m_vecFrm;
    int m_CurAnimIdx;
    int m_AnimFPS;

public:
    virtual void init() override;
    virtual void tick() override{};
    virtual void finaltick() override{};
    void render(bool* open);

private:
    virtual void render() override;

private:
    void DrawViewprot();
    void DrawDetails();
    void DrawSpriteList();

    void DrawAnimationEdit();
    void DrawAnimationList();

public:
    CSpriteEditor();
    virtual ~CSpriteEditor();
};
