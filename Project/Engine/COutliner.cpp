#include "pch.h"
#include "COutliner.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CRenderMgr.h"
#include "CEditorMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "components.h"

#include "CTexture.h"
#include "CAnim.h"
#include "CMaterial.h"

#include "CKeyMgr.h"

COutliner::COutliner()
    : m_DefaultTreeNodeFlag(ImGuiTreeNodeFlags_None)
{
}

COutliner::~COutliner()
{
}

void COutliner::init()
{
    m_DefaultTreeNodeFlag = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
}

void COutliner::render()
{
    ImGui_SetWindowClass_LevelEditor();
    ImGui::Begin("Outliner");

    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        CLayer* layer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i);
        const vector<CGameObject*>& objs = layer->GetParentObject();

        // 각 오브젝트를 돌면서 오브젝트와 현재 레이어를 인자로 DrawNode() 호출
        std::for_each(objs.begin(), objs.end(), [&](CGameObject* obj) { DrawNode(obj); });
    }

    // Outliner 내에서 트리 이외의 부분 마우스 왼쪽 버튼 클릭시 선택오브젝트 초기화
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        CEditorMgr::GetInst()->SetSelectedObject(nullptr);

    // Right-click on blank space
    if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
        if (ImGui::MenuItem("Spawn GameObject"))
        {
            CGameObject* pObj = new CGameObject;
            pObj->SetName(L"Object");
            pObj->AddComponent(new CTransform);

            // 카메라위치 기준 생성
            CCamera* pCam = CRenderMgr::GetInst()->GetCamera(0);
            Vec3 pos = pCam->Transform()->GetWorldPos();
            Vec3 dir = pCam->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            pos += dir.Normalize() * 500.f;
            pObj->Transform()->SetRelativePos(pos);

            GamePlayStatic::SpawnGameObject(pObj, 0);
        }

        ImGui::EndPopup();
    }

    // Drag & Drop
    ImRect inner_rect = ImGui::GetCurrentWindow()->InnerRect;
    if (ImGui::BeginDragDropTargetCustom(inner_rect, ImGui::GetID("Outliner")))
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_COUTLINER"))
        {
            DWORD_PTR data = *((DWORD_PTR*)payload->Data);
            CGameObject* pChild = (CGameObject*)data;
            GamePlayStatic::AddChildObject(nullptr, pChild);
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();

    ImGui_SetWindowClass_LevelEditor();
    ImGui::Begin("Details##Outliner");
    if (nullptr != CEditorMgr::GetInst()->GetSelectedObject())
        DrawDetails(CEditorMgr::GetInst()->GetSelectedObject());

    ImGui::End();
}

void COutliner::DrawNode(CGameObject* obj)
{
    CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();

    int id = -1;

    if (nullptr != pSelectedObj)
        id = pSelectedObj->GetID();

    ImGuiTreeNodeFlags flags =
        ((id == obj->GetID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

    string name = ToString(obj->GetName());

    bool opened = ImGui::TreeNodeEx((void*)(intptr_t)obj->GetID(), flags, name.c_str());

    // Drag & Drop
    if (ImGui::BeginDragDropSource())
    {
        ImGui::Text("%s", ToString(obj->GetName()).c_str());

        ImGui::SetDragDropPayload("LEVEL_EDITOR_COUTLINER", &obj, sizeof(DWORD_PTR));
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_COUTLINER"))
        {
            DWORD_PTR data = *((DWORD_PTR*)payload->Data);
            CGameObject* pChild = (CGameObject*)data;
            GamePlayStatic::AddChildObject(obj, pChild);
        }

        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        CEditorMgr::GetInst()->SetSelectedObject(obj);
    }

    // Add Child PopUp
    string PopUpID = "Add Child Object##";
    PopUpID += std::to_string(obj->GetID());

    ImGui::OpenPopupOnItemClick(PopUpID.c_str(), ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup(PopUpID.c_str()))
    {
        if (ImGui::MenuItem("Add Child Object"))
        {
            CGameObject* pObj = new CGameObject;
            pObj->SetName(L"Child Object");
            pObj->AddComponent(new CTransform);

            GamePlayStatic::AddChildObject(obj, pObj);
        }

        ImGui::EndPopup();
    }

    if (opened)
    {
        // 자식 오브젝트 DrawNode() 호출
        const vector<CGameObject*>& objs = obj->GetChildObject();
        std::for_each(objs.begin(), objs.end(), [&](CGameObject* obj) { DrawNode(obj); });

        ImGui::TreePop();
    }
}

void COutliner::DrawDetails(CGameObject* obj)
{
    // Tag
    {
        string name = ToString(obj->GetName());
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), name.c_str());
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            obj->SetName(ToWstring(buffer));
        }
    }

    // AddComponent
    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent"))
    {
        for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
        {
            if (ImGui::MenuItem(COMPONENT_TYPE_STRING[i]))
            {
                GamePlayStatic::AddComponent(CEditorMgr::GetInst()->GetSelectedObject(), (COMPONENT_TYPE)i);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

    // Layer
    {
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

        vector<string> LayerNames;

        for (int i = 0; i < LAYER_MAX; i++)
        {
            LayerNames.push_back(ToString(pCurLevel->GetLayer(i)->GetName()));
        }

        int LayerIdx = obj->GetLayerIdx();
        string name = LayerNames[LayerIdx];

        if (ImGui_ComboUI(ImGui_LabelPrefix("Layer").c_str(), name, LayerNames))
        {
            GamePlayStatic::LayerChange(obj, pCurLevel->GetLayer(ToWstring(name))->GetLayerIdx());
        }
    }

    DrawTransform(obj);
    DrawCollider2D(obj);
    DrawCollider3D(obj);
    DrawAnimator2D(obj);
    DrawAnimator3D(obj);
    DrawLight2D(obj);
    DrawLight3D(obj);
    DrawCamera(obj);
    DrawMeshRender(obj);
    DrawTileMap(obj);
    DrawParticlesystem(obj);
    DrawSkybox(obj);
    DrawLandscape(obj);
}

void COutliner::DrawTransform(CGameObject* obj)
{
    CTransform* pTr = obj->Transform();
    if (nullptr == pTr)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CTransform).hash_code(),
                                  m_DefaultTreeNodeFlag | ImGuiTreeNodeFlags_DefaultOpen, "Transform");

    ComponentSettingsButton(pTr);

    if (open)
    {
        Vec3 pos = pTr->GetRelativePos();
        ImGui_DrawVec3Control("Location", pos, 10.f);
        pTr->SetRelativePos(pos);

        Vec3 rot = pTr->GetRelativeRotation();
        rot.ToDegree();
        ImGui_DrawVec3Control("Rotation", rot, 1.f);
        rot.ToRadian();
        pTr->SetRelativeRotation(rot);

        Vec3 scale = pTr->GetRelativeScale();
        ImGui_DrawVec3Control("Scale", scale, 1.f, 1.f, D3D11_FLOAT32_MAX, 1.f);
        pTr->SetRelativeScale(scale);

        ImGui::TreePop();
    }
}

void COutliner::DrawCollider2D(CGameObject* obj)
{
    CCollider2D* pCol = obj->Collider2D();
    if (nullptr == pCol)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CCollider2D).hash_code(), m_DefaultTreeNodeFlag, "Collider2D");

    ComponentSettingsButton(pCol);

    if (open)
    {
        const char* Collider2DTypeStrings[] = {"Rect", "Circle"};
        const char* currentCollider2DTypeString = Collider2DTypeStrings[(int)pCol->GetColliderType()];
        if (ImGui::BeginCombo(ImGui_LabelPrefix("Collider2DType").c_str(), currentCollider2DTypeString))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = currentCollider2DTypeString == Collider2DTypeStrings[i];
                if (ImGui::Selectable(Collider2DTypeStrings[i], isSelected))
                {
                    currentCollider2DTypeString = Collider2DTypeStrings[i];
                    pCol->SetColliderType((COLLIDER2D_TYPE)i);
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if (pCol->GetColliderType() == COLLIDER2D_TYPE::RECT)
        {
            bool bAbsolute = pCol->IsAbsolute();
            ImGui::Checkbox(ImGui_LabelPrefix("Absolute").c_str(), &bAbsolute);
            pCol->SetAbsolute(bAbsolute);
        }
        else if (pCol->GetColliderType() == COLLIDER2D_TYPE::CIRCLE)
        {
            float fRadius = pCol->GetRadius();
            if (ImGui::DragFloat(ImGui_LabelPrefix("Radius").c_str(), &fRadius, 1.f, 0.0f, D3D11_FLOAT32_MAX))
                pCol->SetRadius(fRadius);
        }

        Vec2 offsetPos = pCol->GetOffsetPos();
        ImGui::DragFloat2(ImGui_LabelPrefix("Offset Pos").c_str(), &offsetPos.x, 0.1f);
        pCol->SetOffsetPos(offsetPos);

        Vec2 offsetScale = pCol->GetOffsetScale();
        ImGui::DragFloat2(ImGui_LabelPrefix("Offset Scale").c_str(), &offsetScale.x, 0.1f, 0.f, D3D11_FLOAT32_MAX);
        pCol->SetOffsetScale(offsetScale);

        ImGui::TreePop();
    }
}

void COutliner::DrawCollider3D(CGameObject* obj)
{
}

void COutliner::DrawAnimator2D(CGameObject* obj)
{
    // Animator2D
    CAnimator2D* pAni = obj->Animator2D();
    if (nullptr == pAni)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CAnimator2D).hash_code(), m_DefaultTreeNodeFlag, "Animator2D");

    ComponentSettingsButton(pAni);

    if (open)
    {
        const map<wstring, CAnim*>& mapAnim = pAni->GetmapAnim();
        CAnim* pCurAnim = pAni->GetCurAnim();

        if (nullptr != pCurAnim)
        {
            // Animation Names
            vector<string> names;

            for (const auto& iter : mapAnim)
            {
                names.push_back(ToString(iter.first));
            }

            string curAnimName = ToString(pCurAnim->GetName());

            ImGui::Text("Animation Name");
            if (ImGui_ComboUI("##Anim", curAnimName, names))
            {
                pAni->Play(ToWstring(curAnimName), true);
            }

            pCurAnim = pAni->GetCurAnim();

            Ptr<CTexture> pTex = pCurAnim->GetAtlasTex();
            Vec2 TexSize = Vec2((float)pTex->GetWidth(), (float)pTex->GetHeight());
            const vector<tAnimFrm>& vecFrm = pCurAnim->GetVecFrm();

            // Frame Index
            int Frmidx = pCurAnim->GetCurFrmIdx();
            ImGui::Text("Frame Index");
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::SliderInt("##FrmIdx", &Frmidx, 0, (int)vecFrm.size() - 1);
            ImGui::PopItemFlag();

            // Frame Infomation
            ImGui::Text("Frame Infomation");
            static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter |
                                           ImGuiTableFlags_BordersV | ImGuiTableFlags_Reorderable |
                                           ImGuiTableFlags_Hideable | ImGuiTableFlags_SizingFixedSame;
            if (ImGui::BeginTable("##AnimFrame", 3, flags, ImVec2(450.f, 200.f)))
            {
                const tAnimFrm& frm = vecFrm[Frmidx];

                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Frame", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_None);
                ImGui::TableHeadersRow();

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Frame Count");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d", vecFrm.size());

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("LeftTop");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.3f", frm.vLeftTop.x * TexSize.x);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.3f", frm.vLeftTop.y * TexSize.y);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Slice");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.3f", frm.vSlice.x * TexSize.x);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.3f", frm.vSlice.y * TexSize.y);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Offset");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.3f", frm.vOffset.x * TexSize.x);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.3f", frm.vOffset.y * TexSize.y);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Background Size");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.3f", frm.vBackground.x * TexSize.x);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.3f", frm.vBackground.y * TexSize.y);

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Duration");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.3f", frm.Duration);

                ImGui::EndTable();
            }

            // Atlas Texture
            ID3D11ShaderResourceView* pSRV = nullptr;
            pSRV = pTex->GetSRV().Get();

            ImGui::Text("Atlas Texture");
            ImGui::Image((void*)pSRV, ImVec2(TexSize.x, TexSize.y));
        }

        ImGui::TreePop();
    }
}

void COutliner::DrawAnimator3D(CGameObject* obj)
{
}

void COutliner::DrawLight2D(CGameObject* obj)
{
    CLight2D* pLight = obj->Light2D();
    if (nullptr == pLight)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CLight2D).hash_code(), m_DefaultTreeNodeFlag, "Light2D");

    ComponentSettingsButton(pLight);

    if (open)
    {
        const char* LightTypeStrings[] = {"Directional Light", "Point Light", "Spot Light"};
        const char* currentLightTypeStrings = LightTypeStrings[(int)pLight->GetLightType()];
        if (ImGui::BeginCombo(ImGui_LabelPrefix("Light Type").c_str(), currentLightTypeStrings))
        {
            for (int i = 0; i < (UINT)LIGHT_TYPE::END; i++)
            {
                bool isSelected = currentLightTypeStrings == LightTypeStrings[i];
                if (ImGui::Selectable(LightTypeStrings[i], isSelected))
                {
                    currentLightTypeStrings = LightTypeStrings[i];
                    pLight->SetLightType((LIGHT_TYPE)i);
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        Vec4 radiance = pLight->GetLightRadiance();
        if (ImGui::ColorEdit3(ImGui_LabelPrefix("Radiance").c_str(), &radiance.x))
            pLight->SetLightRadiance(radiance);

        float fRadius = pLight->GetRadius();
        float fangle = pLight->GetAngle();

        if (ImGui::DragFloat(ImGui_LabelPrefix("Radius").c_str(), &fRadius, 1.f, 0.0f, D3D11_FLOAT32_MAX))
            pLight->SetRadius(fRadius);

        if (ImGui::SliderFloat(ImGui_LabelPrefix("Angle").c_str(), &fangle, 0.0f, 180.f))
            pLight->SetAngle(fangle);

        float FallOffStart = pLight->GetFallOffStart();
        float FallOffEnd = pLight->GetFallOffEnd();
        float offset = 1.f;

        if (ImGui::SliderFloat(ImGui_LabelPrefix("FallOffStart").c_str(), &FallOffStart, 0.0f, FallOffEnd - offset))
            pLight->SetFallOffStart(FallOffStart);

        if (ImGui::SliderFloat(ImGui_LabelPrefix("FallOffEnd").c_str(), &FallOffEnd, FallOffStart + offset, 10000.f))
            pLight->SetFallOffEnd(FallOffEnd);

        float spotPower = pLight->GetSpotPower();
        if (ImGui::SliderFloat(ImGui_LabelPrefix("Spot Power").c_str(), &spotPower, 1.f, 256.f))
            pLight->SetSpotPower(spotPower);

        ImGui::TreePop();
    }
}

void COutliner::DrawLight3D(CGameObject* obj)
{
    CLight3D* pLight = obj->Light3D();
    if (nullptr == pLight)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CLight3D).hash_code(), m_DefaultTreeNodeFlag, "Light3D");

    ComponentSettingsButton(pLight);

    if (open)
    {
        const char* LightTypeStrings[] = {"Directional Light", "Point Light", "Spot Light"};
        const char* currentLightTypeStrings = LightTypeStrings[(int)pLight->GetLightType()];
        if (ImGui::BeginCombo(ImGui_LabelPrefix("Light Type").c_str(), currentLightTypeStrings))
        {
            for (int i = 0; i < (UINT)LIGHT_TYPE::END; i++)
            {
                bool isSelected = currentLightTypeStrings == LightTypeStrings[i];
                if (ImGui::Selectable(LightTypeStrings[i], isSelected))
                {
                    currentLightTypeStrings = LightTypeStrings[i];
                    pLight->SetLightType((LIGHT_TYPE)i);
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        Vec4 radiance = pLight->GetLightRadiance();
        if (ImGui::ColorEdit3(ImGui_LabelPrefix("Radiance").c_str(), &radiance.x, ImGuiColorEditFlags_HDR))
            pLight->SetLightRadiance(radiance);

        float fRadius = pLight->GetRadius();
        float fangle = pLight->GetAngle();

        if (ImGui::DragFloat(ImGui_LabelPrefix("Radius").c_str(), &fRadius, 1.f, 0.0f, D3D11_FLOAT32_MAX))
            pLight->SetRadius(fRadius);

        if (ImGui::SliderFloat(ImGui_LabelPrefix("Angle").c_str(), &fangle, 0.0f, XM_PI))
            pLight->SetAngle(fangle);

        float FallOffStart = pLight->GetFallOffStart();
        float FallOffEnd = pLight->GetFallOffEnd();
        float offset = 1.f;

        if (ImGui::SliderFloat(ImGui_LabelPrefix("FallOffStart").c_str(), &FallOffStart, 0.0f, FallOffEnd - offset))
            pLight->SetFallOffStart(FallOffStart);

        if (ImGui::SliderFloat(ImGui_LabelPrefix("FallOffEnd").c_str(), &FallOffEnd, FallOffStart + offset, 10000.f))
            pLight->SetFallOffEnd(FallOffEnd);

        float spotPower = pLight->GetSpotPower();
        if (ImGui::SliderFloat(ImGui_LabelPrefix("Spot Power").c_str(), &spotPower, 1.f, 256.f))
            pLight->SetSpotPower(spotPower);

        ImGui::TreePop();
    }
}

void COutliner::DrawCamera(CGameObject* obj)
{
    CCamera* pCam = obj->Camera();
    if (nullptr == pCam)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CCamera).hash_code(), m_DefaultTreeNodeFlag, "Camera");

    ComponentSettingsButton(pCam);

    if (open)
    {
        const char* projectionTypeStrings[] = {"Orthographic", "Perspective"};
        const char* currentProjectionTypeString = projectionTypeStrings[(int)pCam->GetProjType()];
        if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
        {
            for (int i = 0; i < 2; i++)
            {
                bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                {
                    currentProjectionTypeString = projectionTypeStrings[i];
                    pCam->SetProjType((PROJ_TYPE)i);

                    // Rotation 초기화
                    obj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if (pCam->GetProjType() == PROJ_TYPE::PERSPECTIVE)
        {
            float fov = pCam->GetFOV();
            float Degree = XMConvertToDegrees(fov);
            if (ImGui::DragFloat("FOV", &Degree, XM_PI / 18.f)) // 스피드 : 10도
                pCam->SetFOV(XMConvertToRadians(Degree));

            float Near = pCam->GetNear();
            float Far = pCam->GetFar();
            float offset = 1.f;

            if (ImGui::DragFloat("Near", &Near, 1.f, 1.f, Far - offset))
                pCam->SetNear(Near);

            if (ImGui::DragFloat("Far", &Far, 1.f, Near + offset, 10000.f))
                pCam->SetFar(Far);
        }

        if (pCam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
        {
            float scale = pCam->GetScale();
            if (ImGui::DragFloat("Scale", &scale, 0.01f, 1e-5f, 1000.f))
                pCam->SetScale(scale);

            float Near = pCam->GetNear();
            float Far = pCam->GetFar();
            float offset = 1.f;

            if (ImGui::DragFloat("Near", &Near, 1.f, 1.f, Far - offset))
                pCam->SetNear(Near);

            if (ImGui::DragFloat("Far", &Far, 1.f, Near + offset, 10000.f))
                pCam->SetFar(Far);
        }

        float speed = pCam->GetCameraSpeed();
        if (ImGui::DragFloat("Speed", &speed, 1.f, 0.f, 10000.f))
            pCam->SetCameraSpeed(speed);

        ImGui::TreePop();
    }
}

void COutliner::DrawMeshRender(CGameObject* obj)
{
    // MeshRender
    CMeshRender* pMeshRender = obj->MeshRender();
    if (nullptr == pMeshRender)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CMeshRender).hash_code(), m_DefaultTreeNodeFlag, "MeshRender");

    ComponentSettingsButton(pMeshRender);

    if (open)
    {
        Ptr<CMesh> pMesh = pMeshRender->GetMesh();
        Ptr<CMaterial> pMaterial = pMeshRender->GetMaterial();

        // Mesh
        if (ImGui::TreeNodeEx((void*)typeid(CMesh).hash_code(), m_DefaultTreeNodeFlag | ImGuiTreeNodeFlags_DefaultOpen,
                              "Mesh"))
        {
            string name;
            if (nullptr != pMesh)
                name = ToString(pMesh->GetName());

            ImGui_InputText("Mesh", name);

            // Drag & Drop
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_ASSETS"))
                {
                    string name = (char*)payload->Data;
                    name.resize(payload->DataSize);
                    pMeshRender->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(ToWstring(name)));
                }

                ImGui::EndDragDropTarget();
            }

            ImGui::TreePop();
        }

        // Material
        if (ImGui::TreeNodeEx((void*)typeid(CMaterial).hash_code(),
                              m_DefaultTreeNodeFlag | ImGuiTreeNodeFlags_DefaultOpen, "Material"))
        {
            string name;
            if (nullptr != pMaterial)
                name = ToString(pMaterial->GetName());

            ImGui_InputText("Material", name);

            // Drag & Drop
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_ASSETS"))
                {
                    string name = (char*)payload->Data;
                    name.resize(payload->DataSize);
                    pMeshRender->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(ToWstring(name)));
                }

                ImGui::EndDragDropTarget();
            }

            if (ImGui_AlignButton("Material Editor", 1.f))
            {
                CEditorMgr::GetInst()->GetLevelEditor()->ShowMaterialEditor(true);
                CEditorMgr::GetInst()->GetMaterialEditor()->SetMaterial(pMaterial);
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

void COutliner::DrawTileMap(CGameObject* obj)
{
    CTileMap* pTilemap = obj->TileMap();
    if (nullptr == pTilemap)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CTileMap).hash_code(), m_DefaultTreeNodeFlag, "TileMap");

    ComponentSettingsButton(pTilemap);

    if (open)
    {
        Ptr<CMesh> pMesh = pTilemap->GetMesh();
        Ptr<CMaterial> pMaterial = pTilemap->GetMaterial();

        // Mesh
        if (ImGui::TreeNodeEx((void*)typeid(CMesh).hash_code(), m_DefaultTreeNodeFlag, "Mesh"))
        {
            string name;
            if (nullptr != pMesh)
                name = ToString(pMesh->GetName());

            ImGui_InputText("Mesh", name);

            ImGui::TreePop();
        }

        // Material
        if (ImGui::TreeNodeEx((void*)typeid(CMaterial).hash_code(), m_DefaultTreeNodeFlag, "Material"))
        {
            string name;
            if (nullptr != pMaterial)
                name = ToString(pMaterial->GetName());

            ImGui_InputText("Material", name);

            ImGui::TreePop();
        }

        // TileMap
        if (ImGui::TreeNodeEx("TileMap Info", m_DefaultTreeNodeFlag | ImGuiTreeNodeFlags_DefaultOpen, "TileMap Info"))
        {
            int TileCountX = pTilemap->GetTileCountX();
            int TileCountY = pTilemap->GetTileCountY();
            Vec2 TileRenderSize = pTilemap->GetTileRenderSize();
            Ptr<CTexture> pTileAtlas = pTilemap->GetTileAtlas();
            Vec2 TilePixelSize = pTilemap->GetTilePixelSize();

            ImGui::InputInt(ImGui_LabelPrefix("Tile Count X").c_str(), &TileCountX, 1, 100,
                            ImGuiInputTextFlags_ReadOnly);
            ImGui::InputInt(ImGui_LabelPrefix("Tile Count Y").c_str(), &TileCountY, 1, 100,
                            ImGuiInputTextFlags_ReadOnly);
            ImGui::DragFloat2(ImGui_LabelPrefix("Tile Render Size").c_str(), &TileRenderSize.x, 0.f, 0.f, 0.f, "%.3f",
                              ImGuiInputTextFlags_ReadOnly);

            if (nullptr != pTileAtlas)
            {
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                string name = ToString(pTileAtlas->GetKey());
                strcpy_s(buffer, sizeof(buffer), name.c_str());
                ImGui::InputText(ImGui_LabelPrefix("Tile Atlas Name").c_str(), buffer, sizeof(buffer),
                                 ImGuiInputTextFlags_ReadOnly);

                ImGui::DragFloat2(ImGui_LabelPrefix("Tile Pixel Size").c_str(), &TilePixelSize.x, 0.f, 0.f, 0.f, "%.3f",
                                  ImGuiInputTextFlags_ReadOnly);
            }

            ImGui::TreePop();
        }

        ImGui::Separator();

        if (ImGui_AlignButton("TileMap Editor", 1.f))
        {
            CEditorMgr::GetInst()->GetLevelEditor()->ShowTileMapEditor(true);
            CEditorMgr::GetInst()->GetTileMapEditor()->SetTileMap(pTilemap);
        }

        ImGui::TreePop();
    }
}

void COutliner::DrawParticlesystem(CGameObject* obj)
{
    CParticleSystem* pParticleSystem = obj->ParticleSystem();
    if (nullptr == pParticleSystem)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CParticleSystem).hash_code(), m_DefaultTreeNodeFlag, "ParticleSystem");

    ComponentSettingsButton(pParticleSystem);

    if (open)
    {
        Ptr<CMesh> pMesh = pParticleSystem->GetMesh();
        Ptr<CMaterial> pMaterial = pParticleSystem->GetMaterial();

        // Mesh
        if (ImGui::TreeNodeEx((void*)typeid(CMesh).hash_code(), m_DefaultTreeNodeFlag, "Mesh"))
        {
            string name;
            if (nullptr != pMesh)
                name = ToString(pMesh->GetName());

            ImGui_InputText("Mesh", name);

            ImGui::TreePop();
        }

        // Material
        if (ImGui::TreeNodeEx((void*)typeid(CMaterial).hash_code(), m_DefaultTreeNodeFlag, "Material"))
        {
            string name;
            if (nullptr != pMaterial)
                name = ToString(pMaterial->GetName());

            ImGui_InputText("Material", name);

            ImGui::TreePop();
        }

        // Particle Module
        if (ImGui::TreeNodeEx((void*)typeid(tParticleModule).hash_code(),
                              m_DefaultTreeNodeFlag | ImGuiTreeNodeFlags_DefaultOpen, "Particle Module"))
        {
            // ============================================
            // Max Count
            // ============================================
            int MaxParticleCount = pParticleSystem->m_MaxParticleCount;
            if (ImGui::InputInt(ImGui_LabelPrefix("Max Particle Count").c_str(), &MaxParticleCount, 1, 100,
                                ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (MaxParticleCount < 0)
                    MaxParticleCount = 0;

                // 파티클 최대 갯수 제한
                if (MaxParticleCount > 1'000'000)
                    MaxParticleCount = 1'000'000;

                // 크기가 더 커진 경우 버퍼 재생성
                if ((int)pParticleSystem->m_MaxParticleCount < MaxParticleCount)
                {
                    pParticleSystem->m_ParticleBuffer->Create(sizeof(tParticle), MaxParticleCount, SB_TYPE::READ_WRITE,
                                                              true);
                }

                pParticleSystem->m_MaxParticleCount = MaxParticleCount;
            };

            // ============================================
            // RadioButton
            // ============================================
            tParticleModule& Module = pParticleSystem->m_Module;

            if (ImGui::RadioButton("Spawn", Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] == 1))
            {
                if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] > 0)
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 0;
                else
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Drag", Module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] == 1))
            {
                if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] > 0)
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 0;
                else
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Scale", Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] == 1))
            {
                if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] > 0)
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 0;
                else
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE] = 1;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Add Veclocity", Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] == 1))
            {
                if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] > 0)
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 0;
                else
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
            }

            if (ImGui::RadioButton("Noise Force", Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] == 1))
            {
                if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] > 0)
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 0;
                else
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Calculate Force",
                                   Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] == 1))
            {
                if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] > 0)
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 0;
                else
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] = 1;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Render", Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] == 1))
            {
                if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] > 0)
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 0;
                else
                    Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
            }

            // ============================================
            // Tree Node
            // ============================================
            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN])
            {
                if (ImGui::TreeNodeEx("Spawn Module##Particlesystem", m_DefaultTreeNodeFlag, "Spawn Module"))
                {
                    ImGui::Text("Space Type");
                    ImGui::SameLine();
                    ImGui::RadioButton("Local Space", (int*)&Module.SpaceType, 0);
                    ImGui::SameLine();
                    ImGui::RadioButton("World Space", (int*)&Module.SpaceType, 1);

                    ImGui::ColorEdit4(ImGui_LabelPrefix("Color").c_str(), &Module.vSpawnColor.x);
                    ImGui::DragFloat3(ImGui_LabelPrefix("Min Scale").c_str(), &Module.vSpawnMinScale.x, 1.f, 0.f,
                                      D3D11_FLOAT32_MAX);
                    ImGui::DragFloat3(ImGui_LabelPrefix("Max Scale").c_str(), &Module.vSpawnMaxScale.x, 1.f, 0.f,
                                      D3D11_FLOAT32_MAX);

                    ImGui::DragFloat(ImGui_LabelPrefix("Min Life").c_str(), &Module.MinLife, 0.1f, 0.f, Module.MaxLife);
                    ImGui::DragFloat(ImGui_LabelPrefix("Max Life").c_str(), &Module.MaxLife, 0.1f, Module.MinLife,
                                     D3D11_FLOAT32_MAX);
                    ImGui::DragFloat(ImGui_LabelPrefix("Min Mass").c_str(), &Module.MinMass, 0.1f, 0.f, Module.MaxMass);
                    ImGui::DragFloat(ImGui_LabelPrefix("Max Mass").c_str(), &Module.MaxMass, 0.1f, Module.MinMass,
                                     D3D11_FLOAT32_MAX);

                    if (ImGui::InputInt(ImGui_LabelPrefix("Spawn Rate").c_str(), &Module.SpawnRate, 1, 100,
                                        ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (Module.SpawnRate < 0)
                            Module.SpawnRate = 0;
                    }

                    ImGui::Separator();

                    ImGui::Text("Spawn Shape");
                    ImGui::SameLine();
                    ImGui::RadioButton("Sphere", (int*)&Module.SpawnShape, 0);
                    ImGui::SameLine();
                    ImGui::RadioButton("Box", (int*)&Module.SpawnShape, 1);

                    // Spawn Shape - Sphere
                    if (0 == Module.SpawnShape)
                    {
                        ImGui::DragFloat(ImGui_LabelPrefix("Radius").c_str(), &Module.Radius, 1.f, 0.f,
                                         D3D11_FLOAT32_MAX);
                    }
                    // Spawn Shape - Box
                    else if (1 == Module.SpawnShape)
                    {
                        ImGui::DragFloat3(ImGui_LabelPrefix("Spawn Box Scale").c_str(), &Module.vSpawnBoxScale.x, 1.f,
                                          0.f, D3D11_FLOAT32_MAX);
                    }

                    ImGui::TreePop();
                }
            }

            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG])
            {
                if (ImGui::TreeNodeEx("Drag Module##Particlesystem", m_DefaultTreeNodeFlag, "Drag Module"))
                {
                    ImGui::DragFloat(ImGui_LabelPrefix("Drag Time").c_str(), &Module.DragTime, 1.f, 0.f,
                                     D3D11_FLOAT32_MAX);
                    ImGui::TreePop();
                }
            }

            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE])
            {
                if (ImGui::TreeNodeEx("Scale Module##Particlesystem", m_DefaultTreeNodeFlag, "Scale Module"))
                {
                    ImGui::DragFloat3(ImGui_LabelPrefix("Scale Ratio").c_str(), &Module.vScaleRatio.x, 1.f, 0.f,
                                      D3D11_FLOAT32_MAX);
                    ImGui::TreePop();
                }
            }

            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY])
            {
                if (ImGui::TreeNodeEx("Add Velocity Module##Particlesystem", m_DefaultTreeNodeFlag,
                                      "Add Velocity Module"))
                {
                    ImGui::Text("Velocity Type");
                    ImGui::SameLine();
                    ImGui::RadioButton("From Center", (int*)&Module.AddVelocityType, 0);
                    ImGui::SameLine();
                    ImGui::RadioButton("To Center", (int*)&Module.AddVelocityType, 1);
                    ImGui::SameLine();
                    ImGui::RadioButton("Fix Direction", (int*)&Module.AddVelocityType, 2);

                    ImGui::DragFloat(ImGui_LabelPrefix("Min Speed").c_str(), &Module.MinSpeed, 0.1f, 0.f,
                                     Module.MaxSpeed);
                    ImGui::DragFloat(ImGui_LabelPrefix("Max Speed").c_str(), &Module.MaxSpeed, 0.1f, Module.MinSpeed,
                                     D3D11_FLOAT32_MAX);

                    if (2 == Module.AddVelocityType)
                    {
                        ImGui::DragFloat(ImGui_LabelPrefix("Fixed Angle").c_str(), &Module.FixedAngle, 1.f, 0.f, 180.f);
                        ImGui::DragFloat3(ImGui_LabelPrefix("Fixed Direction").c_str(), &Module.vFixedDirection.x, 0.1f,
                                          -1.f, 1.f);
                    }

                    ImGui::TreePop();
                }
            }

            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE])
            {
                if (ImGui::TreeNodeEx("Noise Force##Particlesystem", m_DefaultTreeNodeFlag, "Noise Force"))
                {
                    ImGui::DragFloat(ImGui_LabelPrefix("Noise Force Scale").c_str(), &Module.NoiseForceScale, 1.f, 0.f,
                                     D3D11_FLOAT32_MAX);
                    ImGui::DragFloat(ImGui_LabelPrefix("Noise Force Term").c_str(), &Module.NoiseForceTerm, 0.1f, 0.f,
                                     D3D11_FLOAT32_MAX);
                    ImGui::TreePop();
                }
            }

            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER])
            {
                if (ImGui::TreeNodeEx("Render##Particlesystem", m_DefaultTreeNodeFlag, "Render"))
                {
                    if (ImGui::RadioButton(ImGui_LabelPrefix("Velocity Alignment").c_str(), Module.VelocityAlignment))
                    {
                        if (0 == Module.VelocityAlignment)
                            Module.VelocityAlignment = 1;
                        else if (1 == Module.VelocityAlignment)
                            Module.VelocityAlignment = 0;
                    }

                    ImGui::Text("Velocity Type");
                    ImGui::SameLine();
                    ImGui::RadioButton("Off", (int*)&Module.AlphaBasedLife, 0);
                    ImGui::SameLine();
                    ImGui::RadioButton("Normalized Age", (int*)&Module.AlphaBasedLife, 1);
                    ImGui::SameLine();
                    ImGui::RadioButton("Max Age", (int*)&Module.AlphaBasedLife, 2);

                    if (2 == Module.AlphaBasedLife)
                    {
                        ImGui::DragFloat(ImGui_LabelPrefix("Alpha Max Age").c_str(), &Module.AlphaMaxAge, 0.1f, 0.f,
                                         Module.MaxLife);
                    }

                    ImGui::TreePop();
                }
            }

            // ============================================
            // Texture
            // ============================================
            ImGui::Separator();

            ImGui::Text("Particle Texture");
            void* TextureID = nullptr;

            if (nullptr != pParticleSystem->m_ParticleTex)
                TextureID = pParticleSystem->m_ParticleTex->GetSRV().Get();
            else
                TextureID = CAssetMgr::GetInst()->FindAsset<CTexture>(L"missing_texture")->GetSRV().Get();

            ImGui::Image(TextureID, ImVec2(256.f, 256.f));

            if (nullptr != pParticleSystem->m_ParticleTex)
            {
                if (ImGui::BeginItemTooltip())
                {
                    ImGui::Text("%s", ToString(pParticleSystem->m_ParticleTex->GetKey()).c_str());
                    ImGui::EndTooltip();
                }
            }

            // Drag & Drop
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_ASSETS"))
                {
                    string name = (char*)payload->Data;
                    name.resize(payload->DataSize);
                    pParticleSystem->m_ParticleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(name));
                }

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                {
                    string name = (char*)payload->Data;
                    name.resize(payload->DataSize);
                    pParticleSystem->m_ParticleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(ToWstring(name));
                }

                ImGui::EndDragDropTarget();
            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

void COutliner::DrawSkybox(CGameObject* obj)
{
}

void COutliner::DrawLandscape(CGameObject* obj)
{
}
