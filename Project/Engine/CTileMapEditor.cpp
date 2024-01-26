#include "pch.h"
#include "CTileMapEditor.h"

CTileMapEditor::CTileMapEditor()
    : CEditor(EDITOR_TYPE::TILEMAP)
    , m_TileMap(nullptr)
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
    ImGuiSetWindowClass_TileMapEditor();
    DrawViewport();

    ImGuiSetWindowClass_TileMapEditor();
    DrawDetails();
}

void CTileMapEditor::DrawViewport()
{
    ImGui::Begin("Viewport##TileMapEditor");
    ImGui::End();
}

void CTileMapEditor::DrawDetails()
{
    ImGui::Begin("Details##TileMapEditor");


    ImGui::End();
}
