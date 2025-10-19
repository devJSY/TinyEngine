#include "pch.h"
#include "CPhysicMaterialEditor.h"

CPhysicMaterialEditor::CPhysicMaterialEditor()
    : CEditor(EDITOR_TYPE::PHYSIC_MATERIAL)
{
}

CPhysicMaterialEditor::~CPhysicMaterialEditor()
{
}

void CPhysicMaterialEditor::render(bool* open)
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

    ImGuiID dockSpace = ImGui::GetID("Physic Material Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // Material Editor Render
    // =====================================
    render();

    ImGui::End();
}

void CPhysicMaterialEditor::render()
{
    if (nullptr == m_Mtrl.Get())
        return;

    ImGui_SetWindowClass(GetEditorType());
    DrawDetails();
}

void CPhysicMaterialEditor::DrawDetails()
{
    ImGui::Begin("Details##Physics2DMaterialEditor");
    ImGui_InputText("Material Name", ToString(m_Mtrl->GetName()));

    float DynamicFriction = m_Mtrl->GetDynamicFriction();
    if (ImGui::DragFloat(ImGui_LabelPrefix("Dynamic Friction").c_str(), &DynamicFriction, 0.001f, 0.f, 1.f))
        m_Mtrl->SetDynamicFriction(DynamicFriction);

    float StaticFriction = m_Mtrl->GetStaticFriction();
    if (ImGui::DragFloat(ImGui_LabelPrefix("Static Friction").c_str(), &StaticFriction, 0.001f, 0.f, 1.f))
        m_Mtrl->SetStaticFriction(StaticFriction);

    float Bounciness = m_Mtrl->GetBounciness();
    if (ImGui::DragFloat(ImGui_LabelPrefix("Bounciness").c_str(), &Bounciness, 0.001f, 0.f, 1.f))
        m_Mtrl->SetBounciness(Bounciness);

    static vector<string> vecCombineMode = {"Average", "Minimum", "Maximum", "Mutiply"};

    string FrictionCombineStr = vecCombineMode[(UINT)m_Mtrl->GetFrictionCombine()];
    if (ImGui_ComboUI(ImGui_LabelPrefix("Friction Combine").c_str(), FrictionCombineStr, vecCombineMode))
    {
        if (vecCombineMode[0] == FrictionCombineStr)
            m_Mtrl->SetFrictionCombine(PHYSICMATERIAL_COMBINEMODE::Average);
        else if (vecCombineMode[1] == FrictionCombineStr)
            m_Mtrl->SetFrictionCombine(PHYSICMATERIAL_COMBINEMODE::Minimum);
        else if (vecCombineMode[2] == FrictionCombineStr)
            m_Mtrl->SetFrictionCombine(PHYSICMATERIAL_COMBINEMODE::Maximum);
        else if (vecCombineMode[3] == FrictionCombineStr)
            m_Mtrl->SetFrictionCombine(PHYSICMATERIAL_COMBINEMODE::Mutiply);
    }

    string BounceCombineStr = vecCombineMode[(UINT)m_Mtrl->GetBounceCombine()];
    if (ImGui_ComboUI(ImGui_LabelPrefix("Bounce Combine").c_str(), BounceCombineStr, vecCombineMode))
    {
        if (vecCombineMode[0] == BounceCombineStr)
            m_Mtrl->SetBounceCombine(PHYSICMATERIAL_COMBINEMODE::Average);
        else if (vecCombineMode[1] == BounceCombineStr)
            m_Mtrl->SetBounceCombine(PHYSICMATERIAL_COMBINEMODE::Minimum);
        else if (vecCombineMode[2] == BounceCombineStr)
            m_Mtrl->SetBounceCombine(PHYSICMATERIAL_COMBINEMODE::Maximum);
        else if (vecCombineMode[3] == BounceCombineStr)
            m_Mtrl->SetBounceCombine(PHYSICMATERIAL_COMBINEMODE::Mutiply);
    }

    ImGui::End();
}
