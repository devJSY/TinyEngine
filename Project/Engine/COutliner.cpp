#include "pch.h"
#include "COutliner.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

COutliner::COutliner()
    : m_SelectedObj(nullptr)
{
}

COutliner::~COutliner()
{
}

void COutliner::DrawNode(CGameObject* obj, UINT LayerNum)
{
    // wstring To string
    string str;
    str.assign(obj->GetName().begin(), obj->GetName().end());
    ImGui::Text("%s", str.c_str());
    ImGui::Text("Layer Number : %d", LayerNum);
}

void COutliner::begin()
{
}

void COutliner::render()
{
    ImGui::Begin("Outliner");

    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        CLayer* layer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i);
        const vector<CGameObject*>& objs = layer->GetParentObject();
        std::for_each(objs.begin(), objs.end(), [&](CGameObject* obj) { DrawNode(obj, i); });
    }

    ImGui::End();
}
