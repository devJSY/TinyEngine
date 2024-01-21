#pragma once
#include "CEditor.h"
#include <imgui_node_editor\\imgui_canvas.h>

class CSpriteEditor : public CEditor
{
private:
    Ptr<CTexture> m_pTex;

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

public:
    CSpriteEditor();
    virtual ~CSpriteEditor();
};
