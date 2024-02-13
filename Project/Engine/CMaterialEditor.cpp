#include "pch.h"
#include "CMaterialEditor.h"
#include "CEditorMgr.h"

CMaterialEditor::CMaterialEditor()
    : CEditor(EDITOR_TYPE::MATERIAL)
    , m_Mtrl()
{
}

CMaterialEditor::~CMaterialEditor()
{
}

void CMaterialEditor::render()
{
    if (nullptr == m_Mtrl.Get())
        return;

    ImGui_SetWindowClass_MaterialEditor();
    ImGui::Begin("Details##MaterialEditor");

    ImGui_InputText("Material Name", ToString(m_Mtrl->GetName()));
    ImGui_InputText("Shader Name", ToString(m_Mtrl->GetShader()->GetName()));

    ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
                                             ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;

    // Material Const
    if (ImGui::TreeNodeEx("Material Const##MaterialEditor", DefaultTreeNodeFlag, "Material Const"))
    {
        tMtrlConst& MtrlConst = m_Mtrl->m_Const;

        ImGui::ColorEdit4(ImGui_LabelPrefix("Albedo").c_str(), &MtrlConst.mtrl.vAlbedo.x);
        ImGui::ColorEdit4(ImGui_LabelPrefix("Diffuse").c_str(), &MtrlConst.mtrl.vDiffuse.x);
        ImGui::ColorEdit4(ImGui_LabelPrefix("Specular").c_str(), &MtrlConst.mtrl.vSpecular.x);
        ImGui::SliderFloat(ImGui_LabelPrefix("Metallic").c_str(), &MtrlConst.mtrl.vMetallic, 0.f, 1.f);
        ImGui::SliderFloat(ImGui_LabelPrefix("Roughness").c_str(), &MtrlConst.mtrl.vRoughness, 0.f, 1.f);
        ImGui::ColorEdit4(ImGui_LabelPrefix("Emission").c_str(), &MtrlConst.mtrl.vEmission.x);

        ImGui::Separator();

        static float fDragSpeed = 1.0f;
        ImGui::SliderFloat(ImGui_LabelPrefix("Drag Speed").c_str(), &fDragSpeed, 0.f, 1.f);

        ImGui::Separator();

        ImGui::DragInt(ImGui_LabelPrefix("Int_0").c_str(), &MtrlConst.arrInt[0]);
        ImGui::DragInt(ImGui_LabelPrefix("Int_1").c_str(), &MtrlConst.arrInt[1]);
        ImGui::DragInt(ImGui_LabelPrefix("Int_2").c_str(), &MtrlConst.arrInt[2]);
        ImGui::DragInt(ImGui_LabelPrefix("Int_3").c_str(), &MtrlConst.arrInt[3]);

        ImGui::Separator();

        ImGui::DragFloat(ImGui_LabelPrefix("Float_0").c_str(), &MtrlConst.arrFloat[0], fDragSpeed);
        ImGui::DragFloat(ImGui_LabelPrefix("Float_1").c_str(), &MtrlConst.arrFloat[1], fDragSpeed);
        ImGui::DragFloat(ImGui_LabelPrefix("Float_2").c_str(), &MtrlConst.arrFloat[2], fDragSpeed);
        ImGui::DragFloat(ImGui_LabelPrefix("Float_3").c_str(), &MtrlConst.arrFloat[3], fDragSpeed);

        ImGui::Separator();

        ImGui::DragFloat2(ImGui_LabelPrefix("Vector2_0").c_str(), &MtrlConst.arrVec2[0].x, fDragSpeed);
        ImGui::DragFloat2(ImGui_LabelPrefix("Vector2_1").c_str(), &MtrlConst.arrVec2[1].x, fDragSpeed);
        ImGui::DragFloat2(ImGui_LabelPrefix("Vector2_2").c_str(), &MtrlConst.arrVec2[2].x, fDragSpeed);
        ImGui::DragFloat2(ImGui_LabelPrefix("Vector2_3").c_str(), &MtrlConst.arrVec2[3].x, fDragSpeed);

        ImGui::Separator();

        ImGui::DragFloat4(ImGui_LabelPrefix("Vector4_0").c_str(), &MtrlConst.arrVec4[0].x, fDragSpeed);
        ImGui::DragFloat4(ImGui_LabelPrefix("Vector4_1").c_str(), &MtrlConst.arrVec4[1].x, fDragSpeed);
        ImGui::DragFloat4(ImGui_LabelPrefix("Vector4_2").c_str(), &MtrlConst.arrVec4[2].x, fDragSpeed);
        ImGui::DragFloat4(ImGui_LabelPrefix("Vector4_3").c_str(), &MtrlConst.arrVec4[3].x, fDragSpeed);

        ImGui::TreePop();
    }

    // Texture
    if (ImGui::TreeNodeEx("Texture##MaterialEditor", DefaultTreeNodeFlag, "Texture"))
    {
        constexpr float IMAGE_BASE_SIZE = 250.0f;

        vector<string> vecTexture = {"Texture 0", "Texture 1", "Texture 2", "Texture 3", "Texture 4", "Texture 5"};
        static string CurTexture = vecTexture[0];
        static int TextureIdx = 0;

        if (ImGui_ComboUI(ImGui_LabelPrefix("Textures").c_str(), CurTexture, vecTexture))
        {
            if (CurTexture == vecTexture[0])
                TextureIdx = 0;
            else if (CurTexture == vecTexture[1])
                TextureIdx = 1;
            else if (CurTexture == vecTexture[2])
                TextureIdx = 2;
            else if (CurTexture == vecTexture[3])
                TextureIdx = 3;
            else if (CurTexture == vecTexture[4])
                TextureIdx = 4;
            else if (CurTexture == vecTexture[5])
                TextureIdx = 5;
        }

        Ptr<CTexture> pTex = m_Mtrl->GetTexParam((TEX_PARAM)TextureIdx);
        ID3D11ShaderResourceView* pSRV = nullptr;

        if (nullptr != pTex.Get())
            pSRV = pTex->GetSRV().Get();
        else
            pSRV = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\missing_texture.png", L"Texture\\missing_texture.png")->GetSRV().Get();

        ImGui::Image((void*)pSRV, ImVec2(IMAGE_BASE_SIZE, IMAGE_BASE_SIZE));

        ImGui::OpenPopupOnItemClick("Delete Texture##MaterialEditor", ImGuiPopupFlags_MouseButtonRight);

        if (ImGui::BeginPopup("Delete Texture##MaterialEditor"))
        {
            if (ImGui::MenuItem("Delete Texture"))
            {
                m_Mtrl->SetTexParam((TEX_PARAM)TextureIdx, nullptr);
            }

            ImGui::EndPopup();
        }

        // Drag & Drop
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_ASSETS"))
            {
                string name = (char*)payload->Data;
                name.resize(payload->DataSize);
                m_Mtrl->SetTexParam((TEX_PARAM)TextureIdx, CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(name)));
            }

            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                string name = (char*)payload->Data;
                name.resize(payload->DataSize);
                m_Mtrl->SetTexParam((TEX_PARAM)TextureIdx, CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(name)));
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

void CMaterialEditor::render(bool* open)
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

    ImGuiID dockSpace = ImGui::GetID("Material Editor DockSpace");
    ImGui::DockSpace(dockSpace);

    // =====================================
    // Material Editor Render
    // =====================================
    render();

    ImGui::End();
}
