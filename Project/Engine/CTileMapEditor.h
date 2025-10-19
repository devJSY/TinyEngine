#pragma once
#include "CEditor.h"
#include "CTileMap.h"

enum class DRAW_MODE
{
    PAINT,
    ERASER,
    FILL
};

class CTileMapEditor : public CEditor
{
private:
    CTileMap* m_TileMap;

    ImVec2 m_ViewportOffset;
    float m_ViewportScale;

    int m_SelectedImgIdx;
    DRAW_MODE m_DrawMode;

public:
    virtual void init() override{};
    virtual void tick() override{};
    virtual void finaltick() override;
    virtual void render(bool* open) override;

private:
    void render();

public:
    void SetTileMap(CTileMap* _obj) { m_TileMap = _obj; }

    void DrawViewport();
    void DrawDetails();
    void DrawTileSet();

private:
    void FillTileMap(int _startX, int _startY, int _TargetIdx);

public:
    CTileMapEditor();
    virtual ~CTileMapEditor();
};
