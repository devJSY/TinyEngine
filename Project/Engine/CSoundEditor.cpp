#include "pch.h"
#include "CSoundEditor.h"

CSoundEditor::CSoundEditor()
    : CEditor(EDITOR_TYPE::SOUND)
    , m_Sound(nullptr)
{
}

CSoundEditor::~CSoundEditor()
{
}

void CSoundEditor::render(bool* open)
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

    ImGuiID dockSpace = ImGui::GetID("Sound Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // Material Editor Render
    // =====================================
    render();

    ImGui::End();
}

void CSoundEditor::render()
{
    if (nullptr == m_Sound.Get())
        return;

    ImGui_SetWindowClass(GetEditorType());
    DrawDetails();
}

void CSoundEditor::DrawDetails()
{
    ImGui::Begin("Details##SoundEditor");
    ImGui::End();
}
