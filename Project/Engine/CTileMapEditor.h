#pragma once
#include "CEditor.h"
#include "CTileMap.h"

class CTileMapEditor : public CEditor
{
private:
    CGameObject* m_TileMapObj;
    
    ImVec2 m_ViewportOffset;
    float m_ViewportScale;


public:
    virtual void init() override{};
    virtual void tick() override{};
    virtual void finaltick() override{};
    void render(bool* open);

private:
    virtual void render() override;

public:
    void SetTileMapObj(CGameObject* _obj) { m_TileMapObj = _obj; }

    void DrawViewport();
    void DrawDetails();
    void DrawTileSet();

public:
    CTileMapEditor();
    virtual ~CTileMapEditor();
};
