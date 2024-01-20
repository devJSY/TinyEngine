#include "pch.h"
#include "CSpriteEditor.h"

CSpriteEditor::CSpriteEditor()
    : CEditor(EDITOR_TYPE::SPRITE)
    , m_pAtlas()
{
}

CSpriteEditor::~CSpriteEditor()
{
}

void CSpriteEditor::init()
{
}

void CSpriteEditor::render()
{
}

void CSpriteEditor::render(bool* open)
{
    if (!ImGui::Begin("Sprite Editor", open))
    {
        ImGui::End();
        return;
    }

    render();

    ImGui::End();
}
