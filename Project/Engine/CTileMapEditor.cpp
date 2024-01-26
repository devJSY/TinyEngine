#include "pch.h"
#include "CTileMapEditor.h"

CTileMapEditor::CTileMapEditor()
    : CEditor(EDITOR_TYPE::TILEMAP)
{
}

CTileMapEditor::~CTileMapEditor()
{
}

void CTileMapEditor::render(bool* open)
{
    // =====================================
    // DockSpace
    // =====================================
    ImGuiWindowClass window_class;
    window_class.ClassId = ImGui::GetMainViewport()->ID;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplitOther;
    window_class.DockingAllowUnclassed = true;
    ImGui::SetNextWindowClass(&window_class);

    if (!ImGui::Begin(ToString(GetName()).c_str(), open))
    {
        ImGui::End();
        return;
    }

    ImGuiID dockSpace = ImGui::GetID("TileMap Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // TileMap Editor Render
    // =====================================
    render();

    ImGui::End();
}

void CTileMapEditor::render()
{
    ImGui::Begin("Viewport##TileMapEditor");
    ImGui::End();
}
