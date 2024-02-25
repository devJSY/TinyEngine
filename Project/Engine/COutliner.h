#pragma once
#pragma once
#pragma once
#include "CEntity.h"

class CGameObject;
class CComponent;

class COutliner : public CEntity
{
private:
    ImGuiTreeNodeFlags m_DefaultTreeNodeFlag;

public:
    void init();
    void render();

private:
    void DrawNode(CGameObject* obj);
    void DrawDetails(CGameObject* obj);

private:
    void DrawTransform(CGameObject* obj);
    void DrawCollider2D(CGameObject* obj);
    void DrawCollider3D(CGameObject* obj);
    void DrawAnimator2D(CGameObject* obj);
    void DrawAnimator3D(CGameObject* obj);
    void DrawLight2D(CGameObject* obj);
    void DrawLight3D(CGameObject* obj);
    void DrawCamera(CGameObject* obj);
    void DrawMeshRender(CGameObject* obj);
    void DrawTileMap(CGameObject* obj);
    void DrawParticlesystem(CGameObject* obj);
    void DrawSkybox(CGameObject* obj);
    void DrawLandscape(CGameObject* obj);

private:
    template <typename T>
    void ComponentSettingsButton(T* comp);

    CLONE_DISABLE(COutliner);

private:
    COutliner();
    virtual ~COutliner();

    friend class CLevelEditor; // CLevelEditor 에서만 생성가능하도록 제한
};

template <typename T>
void COutliner::ComponentSettingsButton(T* comp)
{
    ImVec2 contentRegion = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::SameLine(contentRegion.x - lineHeight * 0.5f);
    string id = "+";
    id += "##";
    id += typeid(T).name();
    if (ImGui::Button(id.c_str(), ImVec2{lineHeight, lineHeight}))
    {
        ImGui::OpenPopup(typeid(T).name());
    }

    if (ImGui::BeginPopup(typeid(T).name()))
    {
        if (ImGui::MenuItem("Remove component"))
        {
            GamePlayStatic::RemoveComponent(comp->GetOwner(), comp->GetComponentType());
        }

        ImGui::EndPopup();
    }
}