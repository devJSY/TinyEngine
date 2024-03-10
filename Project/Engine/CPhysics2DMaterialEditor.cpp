#include "pch.h"
#include "CPhysics2DMaterialEditor.h"

CPhysics2DMaterialEditor::CPhysics2DMaterialEditor()
    : CEditor(EDITOR_TYPE::PHYSICS2D_MATERIAL)
{
}

CPhysics2DMaterialEditor::~CPhysics2DMaterialEditor()
{
}

void CPhysics2DMaterialEditor::render(bool* open)
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

    ImGuiID dockSpace = ImGui::GetID("Physics2D Material Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // Material Editor Render
    // =====================================
    render();

    ImGui::End();
}

void CPhysics2DMaterialEditor::render()
{
    if (nullptr == m_Mtrl.Get())
        return;

    ImGui_SetWindowClass_Physics2DMaterialEditor();
    DrawDetails();
}

void CPhysics2DMaterialEditor::DrawDetails()
{
    ImGui::Begin("Details##Physics2DMaterialEditor");
    ImGui_InputText("Material Name", ToString(m_Mtrl->GetName()));

    float Friction = m_Mtrl->GetFriction();
    if (ImGui::DragFloat(ImGui_LabelPrefix("Friction").c_str(), &Friction, 0.001f, 0.f, 1.f))
        m_Mtrl->SetFriction(Friction);

    float Bounciness = m_Mtrl->GetBounciness();
    if (ImGui::DragFloat(ImGui_LabelPrefix("Bounciness").c_str(), &Bounciness, 0.001f, 0.f, 1.f))
        m_Mtrl->SetBounciness(Bounciness);

    ImGui::End();
}
