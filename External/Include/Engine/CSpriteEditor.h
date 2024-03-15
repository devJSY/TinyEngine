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

    CAnim* m_pAnim;
    int m_AnimFPS;
    bool m_bAnimPlay;
    Vec2 m_vAnimBackGround;

    int m_CellWidth;
    int m_CellHeight;

    bool m_bEmptyAutoErase;

public:
    virtual void init() override;
    virtual void tick() override{};
    virtual void finaltick() override;
    virtual void render(bool* open) override;

private:
    void render();

private:
    void DrawViewport();
    void DrawDetails();
    void DrawSpriteList();

    void DrawAnimationViewport();
    void DrawAnimationList();

private:
    int ExtractSprite(tPixel* pPixel, int _x, int _y, int _width, int _height);
    int AlignSprite(tSprite& _sprite);

public:
    CSpriteEditor();
    virtual ~CSpriteEditor();
};
