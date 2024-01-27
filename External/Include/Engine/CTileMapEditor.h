#pragma once
#include "CEditor.h"
#include "CTileMap.h"

class CTileMapEditor : public CEditor
{
private:
    CTileMap* m_TileMap;

    ImVec2 m_ViewportOffset;
    float m_ViewportScale;

    int m_SelectedImgIdx;

public:
    virtual void init() override{};
    virtual void tick() override{};
    virtual void finaltick() override{};
    void render(bool* open);

private:
    virtual void render() override;

public:
    void SetTileMap(CTileMap* _obj) { m_TileMap = _obj; }

    void DrawViewport();
    void DrawDetails();
    void DrawTileSet();

public:
    CTileMapEditor();
    virtual ~CTileMapEditor();
};
