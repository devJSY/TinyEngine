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

    ImGuiTreeNodeFlags DefaultTreeNodeFlag = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
                                             ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;

    // Material Coefficient
    if (ImGui::TreeNodeEx("Material Coefficient##MaterialEditor", DefaultTreeNodeFlag, "Material Coefficient"))
    {
        tMtrlConst& MtrlConst = m_Mtrl->m_Const;

        ImGui::ColorEdit4(ImGui_LabelPrefix("Albedo").c_str(), &MtrlConst.mtrl.vAlbedo.x);
        ImGui::ColorEdit4(ImGui_LabelPrefix("Diffuse").c_str(), &MtrlConst.mtrl.vDiffuse.x);
        ImGui::ColorEdit4(ImGui_LabelPrefix("Specular").c_str(), &MtrlConst.mtrl.vSpecular.x);
        ImGui::SliderFloat(ImGui_LabelPrefix("Metallic").c_str(), &MtrlConst.mtrl.vMetallic, 0.f, 1.f);
        ImGui::SliderFloat(ImGui_LabelPrefix("Roughness").c_str(), &MtrlConst.mtrl.vRoughness, 0.f, 1.f);
        ImGui::ColorEdit4(ImGui_LabelPrefix("Emission").c_str(), &MtrlConst.mtrl.vEmission.x);

        ImGui::TreePop();
    }

    Ptr<CGraphicsShader> pShader = m_Mtrl->GetShader();
    if (nullptr == pShader)
        return;

    // Shader
    if (ImGui::TreeNodeEx("Shader##MaterialEditor", DefaultTreeNodeFlag, "Shader"))
    {
        ImGui_InputText("Shader Name", ToString(pShader->GetName()));

        ImGui::Separator();

        // Scaler
        static float fDragSpeed = 1.0f;
        ImGui::SliderFloat(ImGui_LabelPrefix("Drag Speed").c_str(), &fDragSpeed, 0.f, 1.f);

        const vector<tScalarParam>& ScalerParams = pShader->GetScalarParam();

        for (int i = 0; i < ScalerParams.size(); i++)
        {
            switch (ScalerParams[i].Type)
            {
            case INT_0:
            case INT_1:
            case INT_2:
            case INT_3: {
                ImGui::DragInt(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (int*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                               fDragSpeed);
            }
            break;
            case FLOAT_0:
            case FLOAT_1:
            case FLOAT_2:
            case FLOAT_3: {
                ImGui::DragFloat(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (float*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                 fDragSpeed);
            }
            break;
            case VEC2_0:
            case VEC2_1:
            case VEC2_2:
            case VEC2_3: {
                ImGui::DragFloat2(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (float*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                  fDragSpeed);
            }
            break;
            case VEC4_0:
            case VEC4_1:
            case VEC4_2:
            case VEC4_3: {
                ImGui::DragFloat4(ImGui_LabelPrefix(ScalerParams[i].Desc.c_str()).c_str(), (float*)m_Mtrl->GetScalarParam(ScalerParams[i].Type),
                                  fDragSpeed);
            }
            break;
            case MAT_0:
            case MAT_1:
            case MAT_2:
            case MAT_3: {
            }
            break;
            }
        }

        ImGui::Separator();

        // Texture
        const vector<tTexParam>& TexParams = pShader->GetTexParam();
        constexpr float IMAGE_BASE_SIZE = 250.0f;

        for (int i = 0; i < TexParams.size(); i++)
        {
            Ptr<CTexture> pTex = m_Mtrl->GetTexParam(TexParams[i].Type);
            ID3D11ShaderResourceView* pSRV = nullptr;

            if (nullptr != pTex.Get())
                pSRV = pTex->GetSRV().Get();
            else
                pSRV = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\missing_texture.png", L"Texture\\missing_texture.png")->GetSRV().Get();

            ImGui::Text(TexParams[i].Desc.c_str());
            ImGui::Image((void*)pSRV, ImVec2(IMAGE_BASE_SIZE, IMAGE_BASE_SIZE));

            string PopupID = "Delete Texture##MaterialEditor";
            PopupID += TexParams[i].Desc;
            ImGui::OpenPopupOnItemClick(PopupID.c_str(), ImGuiPopupFlags_MouseButtonRight);

            if (ImGui::BeginPopup(PopupID.c_str()))
            {
                if (ImGui::MenuItem("Delete Texture"))
                {
                    m_Mtrl->SetTexParam(TexParams[i].Type, nullptr);
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
                    m_Mtrl->SetTexParam(TexParams[i].Type, CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(name)));
                }

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                {
                    string name = (char*)payload->Data;
                    name.resize(payload->DataSize);
                    m_Mtrl->SetTexParam(TexParams[i].Type, CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(name)));
                }

                ImGui::EndDragDropTarget();
            }
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
