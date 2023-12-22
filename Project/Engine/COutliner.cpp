#include "pch.h"
#include "COutliner.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CCamera.h"

COutliner::COutliner()
    : m_SelectedObj(nullptr)
{
}

COutliner::~COutliner()
{
}

void COutliner::DrawNode(CGameObject* obj)
{
    UINT id = 2147000000;

    if (nullptr != m_SelectedObj)
        id = m_SelectedObj->GetID();

    ImGuiTreeNodeFlags flags =
        ((id == obj->GetID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

    // wstring To string
    string str;
    str.assign(obj->GetName().begin(), obj->GetName().end());

    bool opened = ImGui::TreeNodeEx((void*)(intptr_t)obj->GetID(), flags, str.c_str());

    if (ImGui::IsItemClicked())
    {
        m_SelectedObj = obj;
    }

    if (opened)
    {
        // 자식 오브젝트 DrawNode() 호출
        const vector<CGameObject*>& objs = obj->GetChildObject();
        std::for_each(objs.begin(), objs.end(), [&](CGameObject* obj) { DrawNode(obj); });

        ImGui::TreePop();
    }
}

static void DrawVec3Control(const std::string& label, Vec3& values, float speed = 0.1f, float min = 0.f,
                            float max = 0.f, float resetValue = 0.0f, float columnWidth = 100.0f)
{
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, speed, min, max, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, speed, min, max, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, speed, min, max, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}

void COutliner::DrawDetails(CGameObject* obj)
{
    // Tag
    {
        string str;
        str.assign(obj->GetName().begin(), obj->GetName().end());

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), str.c_str());
        ImGui::InputText("Tag", buffer, sizeof(buffer));
    }

    // Transform
    CTransform* tr = obj->Transform();
    if (nullptr != tr)
    {
        if (ImGui::TreeNodeEx((void*)typeid(CTransform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
        {
            Vec3 pos = tr->GetRelativePos();
            DrawVec3Control("Location", pos, 100.f);
            tr->SetRelativePos(pos);

            Vec3 rot = tr->GetRelativeRotation();
            DrawVec3Control("Rotation", rot, XM_PI / 180.f);
            tr->SetRelativeRotation(rot);

            Vec3 scale = tr->GetRelativeScale();
            DrawVec3Control("Scale", scale, 10.f, 1.f, 100000.0f, 1.f);
            tr->SetRelativeScale(scale);

            ImGui::TreePop();
        }
    }

    // Camera
    CCamera* cam = obj->Camera();
    if (nullptr != cam)
    {
        if (ImGui::TreeNodeEx((void*)typeid(CCamera).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
        {
            const char* projectionTypeStrings[] = {"Orthographic", "Perspective"};
            const char* currentProjectionTypeString = projectionTypeStrings[(int)cam->GetProjType()];
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                    {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        cam->SetProjType((PROJ_TYPE)i);
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            if (cam->GetProjType() == PROJ_TYPE::PERSPECTIVE)
            {
                float fov = cam->GetFOV();
                float Degree = XMConvertToDegrees(fov);
                if (ImGui::DragFloat("FOV", &Degree, XM_PI / 18.f)) // 스피드 - 10도
                    cam->SetFOV(XMConvertToRadians(Degree));

                float Near = cam->GetNear();
                float Far = cam->GetFar();
                float offset = 1.f;

                if (ImGui::DragFloat("Near", &Near, 1.f, 1.f, Far - offset))
                    cam->SetNear(Near);

                if (ImGui::DragFloat("Far", &Far, 1.f, Near + offset, 10000.f))
                    cam->SetFar(Far);
            }

            if (cam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
            {
                float scale = cam->GetScale();
                if (ImGui::DragFloat("Scale", &scale, 0.01f, 1.f, 100.f))
                    cam->SetScale(scale);

                float Near = cam->GetNear();
                float Far = cam->GetFar();
                float offset = 1.f;

                if (ImGui::DragFloat("Near", &Near, 1.f, 1.f, Far - offset))
                    cam->SetNear(Near);

                if (ImGui::DragFloat("Far", &Far, 1.f, Near + offset, 10000.f))
                    cam->SetFar(Far);
            }

            ImGui::TreePop();
        }
    }
}

void COutliner::begin()
{
}

void COutliner::render()
{
    // 유효성체크
    // IsValid(m_SelectedObj);

    ImGui::Begin("Outliner");

    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        CLayer* layer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i);
        const vector<CGameObject*>& objs = layer->GetParentObject();

        // 각 오브젝트를 돌면서 오브젝트와 현재 레이어를 인자로 DrawNode() 호출
        std::for_each(objs.begin(), objs.end(), [&](CGameObject* obj) { DrawNode(obj); });
    }

    // Outliner 창내에서 트리 이외의 부분 마우스 왼쪽 버튼 클릭시 선택오브젝트 초기화
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_SelectedObj = nullptr;

    ImGui::End();

    ImGui::Begin("Details");
    if (nullptr != m_SelectedObj)
        DrawDetails(m_SelectedObj);
    ImGui::End();
}
