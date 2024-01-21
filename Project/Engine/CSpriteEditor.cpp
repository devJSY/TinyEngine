#include "pch.h"
#include "CSpriteEditor.h"
#include "CAssetMgr.h"

CSpriteEditor::CSpriteEditor()
    : CEditor(EDITOR_TYPE::SPRITE)
    , m_pTex()
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
    ImGuiSetWindowClass_SpriteEditor();
    DrawViewprot();

    ImGuiSetWindowClass_SpriteEditor();
    DrawDetails();

    ImGuiSetWindowClass_SpriteEditor();
    DrawSpriteList();
}

void CSpriteEditor::DrawViewprot()
{
    ImGui::Begin("Viewprot##SpriteEditor");

    if (nullptr != m_pTex.Get())
        ImGui::Image((void*)m_pTex->GetSRV().Get(), ImGui::GetContentRegionAvail());

    ImGui::End();
}

void CSpriteEditor::DrawDetails()
{
    ImGui::Begin("Details##SpriteEditor");

    const map<wstring, Ptr<CAsset>>& mapTextures = CAssetMgr::GetInst()->GetMapAsset(ASSET_TYPE::TEXTURE);
    vector<string> names;
    for (const auto& iter : mapTextures)
    {
        names.push_back(ToString(iter.first));
    }

    std::string CurTextureName;

    if (nullptr != m_pTex.Get())
        CurTextureName = ToString(m_pTex->GetKey());

    if (ImGuiComboUI(ImGuiLabelPrefix("Source Texture").c_str(), CurTextureName, names))
    {
        m_pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(CurTextureName));
    }

    if (nullptr != m_pTex.Get())
        ImGui::Image((void*)m_pTex->GetSRV().Get(), ImVec2(250.f, 250.f));

    ImGui::End();
}

void CSpriteEditor::DrawSpriteList()
{
    ImGui::Begin("Sprite List##SpriteEditor");
    ImGui::End();
}

void CSpriteEditor::render(bool* open)
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

    ImGuiID dockSpace = ImGui::GetID("Sprite Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // Sprite Editor Render
    // =====================================
    render();

    ImGui::End();
}
