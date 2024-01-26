#pragma once
#include "CEditor.h"
#include "CTileMap.h"

class CTileMapEditor : public CEditor
{
private:
    CTileMap* m_TileMap;

public:
    virtual void init() override{};
    virtual void tick() override{};
    virtual void finaltick() override{};
    void render(bool* open);

private:
    virtual void render() override;

public:
    void SetTileMap(CTileMap* _TileMap) { m_TileMap = _TileMap; }

    void DrawViewport();
    void DrawDetails();

public:
    CTileMapEditor();
    virtual ~CTileMapEditor();
};
