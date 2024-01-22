#pragma once
#include "CEditor.h"
#include <imgui_node_editor\\imgui_canvas.h>

class CSpriteEditor : public CEditor
{
private:
    Ptr<CTexture> m_pTex;

    ImVector<ImRect> m_Sprites;
    ImRect m_DragRect;
    ImVec2 m_ViewportOffset;
    float m_ViewportScale;

    int m_SelectedSpriteIdx;
    int m_LineCheckFlag;

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
