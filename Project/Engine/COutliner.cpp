#include "pch.h"
#include "COutliner.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CTransform.h"

COutliner::COutliner()
    : m_SelectedObj(nullptr)
{
}

COutliner::~COutliner()
{
}

void COutliner::DrawNode(CGameObject* obj, UINT LayerNum)
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
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        str += " Test";
        bool opened = ImGui::TreeNodeEx((void*)9817239, flags, str.c_str());
        if (opened)
            ImGui::TreePop();
        ImGui::TreePop();
    }
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
            ImGui::DragFloat3("Location", pos, 100.f);
            tr->SetRelativePos(pos);

            Vec3 rot = tr->GetRelativeRotation();
            ImGui::DragFloat3("Rotation", rot, XM_PI / 180.f);
            tr->SetRelativeRotation(rot);

            Vec3 scale = tr->GetRelativeScale();
            ImGui::DragFloat3("Scale", scale, 1.f);
            tr->SetRelativeScale(scale);

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
        std::for_each(objs.begin(), objs.end(), [&](CGameObject* obj) { DrawNode(obj, i); });
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
