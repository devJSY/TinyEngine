#include "pch.h"
#include "COutliner.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CRenderMgr.h"
#include "CEditorMgr.h"
#include "CPhysics2DMgr.h"
#include <Scripts\\CScriptMgr.h>

#include "CEngine.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "components.h"
#include "CScript.h"

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
    m_DefaultTreeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
}

void COutliner::render()
{
    // ===================
    // Outliner
    // ===================
    ImGui_SetWindowClass(EDITOR_TYPE::LEVEL);
    ImGui::Begin("Outliner");

    static ImGuiTextFilter Filter;
    Filter.Draw("##OutlinerFilter", ImGui::GetContentRegionAvail().x);

    ImGui::Separator();

    // Editor Camera
    CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();
    CGameObject* pEditorCam = CRenderMgr::GetInst()->GetEditorCamera()->GetOwner();
    ImGuiTreeNodeFlags EditCamflags =
        ((pSelectedObj == pEditorCam) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    ImGui::TreeNodeEx((void*)(intptr_t)pEditorCam->GetID(), EditCamflags, ToString(pEditorCam->GetName()).c_str());

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        CEditorMgr::GetInst()->SetSelectedObject(pEditorCam);

    ImGui::Separator();

    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        CLayer* layer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i);
        const vector<CGameObject*>& objs = layer->GetParentObject();

        for (size_t i = 0; i < objs.size(); i++)
        {
            if (!Filter.PassFilter(ToString(objs[i]->GetName()).c_str()))
                continue;

            DrawNode(objs[i]);
        }
    }

    // Outliner 내에서 트리 이외의 부분 마우스 왼쪽 버튼 클릭시 선택오브젝트 초기화
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        CEditorMgr::GetInst()->SetSelectedObject(nullptr);

    // =====================
    // Popup
    // =====================
    if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
    {
        int Dirtyflag = 0;

        if (ImGui::MenuItem("Create Empty Object"))
            Dirtyflag = 1;

        if (ImGui::BeginMenu("2D Object"))
        {
            if (ImGui::MenuItem("Rect"))
                Dirtyflag = 10;

            if (ImGui::MenuItem("Circle"))
                Dirtyflag = 11;

            if (ImGui::BeginMenu("Light"))
            {
                if (ImGui::MenuItem("Directional"))
                    Dirtyflag = 20;
                if (ImGui::MenuItem("Point"))
                    Dirtyflag = 21;
                if (ImGui::MenuItem("Spot"))
                    Dirtyflag = 22;

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Physics"))
            {
                if (ImGui::MenuItem("Box"))
                    Dirtyflag = 30;
                if (ImGui::MenuItem("Circle"))
                    Dirtyflag = 31;

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("3D Object"))
        {
            if (ImGui::MenuItem("Box"))
                Dirtyflag = 40;

            if (ImGui::MenuItem("Sphere"))
                Dirtyflag = 41;

            if (ImGui::BeginMenu("Light"))
            {
                if (ImGui::MenuItem("Directional"))
                    Dirtyflag = 50;
                if (ImGui::MenuItem("Point"))
                    Dirtyflag = 51;
                if (ImGui::MenuItem("Spot"))
                    Dirtyflag = 52;

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (Dirtyflag > 0)
        {
            CGameObject* pObj = new CGameObject;
            pObj->SetName(L"Object");
            pObj->AddComponent(new CTransform);

            if (10 == Dirtyflag)
            {
                pObj->SetName(L"Rect");
                pObj->AddComponent(new CMeshRender);
                pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
                pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
            }
            else if (11 == Dirtyflag)
            {
                pObj->SetName(L"Circle");
                pObj->AddComponent(new CMeshRender);
                pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
                pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
            }
            else if (20 == Dirtyflag)
            {
                pObj->SetName(L"Directional Light2D");
                pObj->AddComponent(new CLight2D);
                pObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
            }
            else if (21 == Dirtyflag)
            {
                pObj->SetName(L"Point Light2D");
                pObj->AddComponent(new CLight2D);
                pObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
            }
            else if (22 == Dirtyflag)
            {
                pObj->SetName(L"Spot Light2D");
                pObj->AddComponent(new CLight2D);
                pObj->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
            }
            else if (30 == Dirtyflag)
            {
                pObj->SetName(L"Physics2D Box Object");
                pObj->AddComponent(new CRigidbody2D);
                pObj->AddComponent(new CBoxCollider2D);
            }
            else if (31 == Dirtyflag)
            {
                pObj->SetName(L"Physics2D Circle Object");
                pObj->AddComponent(new CRigidbody2D);
                pObj->AddComponent(new CCircleCollider2D);
            }
            else if (40 == Dirtyflag)
            {
                pObj->SetName(L"Box");
                pObj->AddComponent(new CMeshRender);
                pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
                pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRMtrl"));
            }
            else if (41 == Dirtyflag)
            {
                pObj->SetName(L"Sphere");
                pObj->AddComponent(new CMeshRender);
                pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
                pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRMtrl"));
            }
            else if (50 == Dirtyflag)
            {
                pObj->SetName(L"Directional Light3D");
                pObj->AddComponent(new CLight3D);
                pObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
            }
            else if (51 == Dirtyflag)
            {
                pObj->SetName(L"Point Light3D");
                pObj->AddComponent(new CLight3D);
                pObj->Light3D()->SetLightType(LIGHT_TYPE::POINT);
            }
            else if (52 == Dirtyflag)
            {
                pObj->SetName(L"Spot Light3D");
                pObj->AddComponent(new CLight3D);
                pObj->Light3D()->SetLightType(LIGHT_TYPE::SPOT);
            }

            // 카메라위치 기준 생성
            CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();
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

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_ASSETS"))
        {
            string AssetStr = (char*)payload->Data;
            AssetStr.resize(payload->DataSize);
            std::filesystem::path AssetPath = AssetStr;
            if (L".pref" == AssetPath.extension())
            {
                Ptr<CPrefab> pPrefab = CAssetMgr::GetInst()->Load<CPrefab>(AssetPath, AssetPath);
                CGameObject* pObj = pPrefab->Instantiate();
                GamePlayStatic::SpawnGameObject(pObj, pObj->GetLayerIdx());
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::End();

    // ===================
    // Details
    // ===================
    ImGui_SetWindowClass(EDITOR_TYPE::LEVEL);
    ImGui::Begin("Details##Outliner");
    if (nullptr != CEditorMgr::GetInst()->GetSelectedObject())
        DrawDetails(CEditorMgr::GetInst()->GetSelectedObject());
    ImGui::End();
}

void COutliner::DrawNode(CGameObject* obj)
{
    CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();

    ImGuiTreeNodeFlags flags = ((pSelectedObj == obj) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

    string name = ToString(obj->GetName());

    if (nullptr != pSelectedObj && pSelectedObj->IsAncestor(obj))
    {
        ImGui::SetNextItemOpen(true);
    }

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

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_ASSETS"))
        {
            string AssetStr = (char*)payload->Data;
            AssetStr.resize(payload->DataSize);
            std::filesystem::path AssetPath = AssetStr;
            if (L".pref" == AssetPath.extension())
            {
                Ptr<CPrefab> pPrefab = CAssetMgr::GetInst()->Load<CPrefab>(AssetPath, AssetPath);
                GamePlayStatic::AddChildObject(obj, pPrefab->Instantiate());
            }
        }

        ImGui::EndDragDropTarget();
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        CEditorMgr::GetInst()->SetSelectedObject(obj);
    }

    // Outliner_PopUp
    string PopUpID = "Outliner_PopUp##";
    PopUpID += std::to_string(obj->GetID());

    ImGui::OpenPopupOnItemClick(PopUpID.c_str(), ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup(PopUpID.c_str()))
    {
        if (ImGui::MenuItem("Create Prefab"))
        {
            Ptr<CPrefab> pPrefab = new CPrefab(obj->Clone());
            wstring path = L"prefab\\" + obj->GetName() + L".pref";
            pPrefab->Save(path);
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
            if (ImGui::MenuItem(COMPONENT_TYPE_STRING[i], NULL, false, nullptr == obj->GetComponent((COMPONENT_TYPE)i)))
            {
                GamePlayStatic::AddComponent(obj, (COMPONENT_TYPE)i);
            }
        }

        if (ImGui::BeginMenu("Script"))
        {
            vector<wstring> vecScriptName;
            CScriptMgr::GetScriptInfo(vecScriptName);

            for (size_t i = 0; i < vecScriptName.size(); i++)
            {
                if (ImGui::MenuItem(ToString(vecScriptName[i]).c_str()))
                {
                    obj->AddComponent(CScriptMgr::GetScript(vecScriptName[i]));
                }
            }

            ImGui::EndMenu();
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
        if (LayerIdx >= 0)
        {
            string name = LayerNames[LayerIdx];

            if (ImGui_ComboUI(ImGui_LabelPrefix("Layer").c_str(), name, LayerNames))
            {
                GamePlayStatic::LayerChange(obj, pCurLevel->GetLayer(ToWstring(name))->GetLayerIdx());
            }
        }
    }

    DrawTransform(obj);
    DrawAnimator2D(obj);
    DrawAnimator3D(obj);
    DrawLight2D(obj);
    DrawLight3D(obj);
    DrawCamera(obj);
    DrawRigidbody2D(obj);
    DrawBoxCollider2D(obj);
    DrawCircleCollider2D(obj);
    DrawMeshRender(obj);
    DrawTileMap(obj);
    DrawParticlesystem(obj);
    DrawSkybox(obj);
    DrawLandscape(obj);
    DrawTextRender(obj);
    DrawScript(obj);
}

void COutliner::DrawTransform(CGameObject* obj)
{
    CTransform* pTr = obj->Transform();
    if (nullptr == pTr)
        return;

    bool open =
        ImGui::TreeNodeEx((void*)typeid(CTransform).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::TRANSFORM]);

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

        ImGui::Spacing();
        ImGui::Separator();

        bool bAbsolute = pTr->IsAbsolute();
        ImGui::Checkbox(ImGui_LabelPrefix("Absolute Scale").c_str(), &bAbsolute);
        pTr->SetAbsolute(bAbsolute);

        ImGui::TreePop();
    }
}

void COutliner::DrawAnimator2D(CGameObject* obj)
{
    // Animator2D
    CAnimator2D* pAnimator = obj->Animator2D();
    if (nullptr == pAnimator)
        return;

    bool open =
        ImGui::TreeNodeEx((void*)typeid(CAnimator2D).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

    ComponentSettingsButton(pAnimator);

    if (open)
    {
        const map<wstring, CAnim*>& mapAnim = pAnimator->GetmapAnim();
        CAnim* pCurAnim = pAnimator->GetCurAnim();

        // =====================
        // Animation Select
        // =====================
        string curAnimName = string();
        if (nullptr != pCurAnim)
            curAnimName = ToString(pCurAnim->GetName());

        ImGui::Text("Animation Name");
        if (ImGui_ComboUI("##Anim", curAnimName, mapAnim))
        {
            pAnimator->Play(ToWstring(curAnimName), true);
        }

        // =====================
        // Animation Info
        // =====================
        if (nullptr != pCurAnim)
        {
            pCurAnim = pAnimator->GetCurAnim();

            Ptr<CTexture> pTex = pCurAnim->GetAtlasTex();
            Vec2 TexSize = Vec2((float)pTex->GetWidth(), (float)pTex->GetHeight());
            const vector<tAnimFrm>& vecFrm = pCurAnim->GetVecFrm();

            // Frame Index
            int Frmidx = pCurAnim->GetCurFrmIdx();
            if (ImGui::SliderInt(ImGui_LabelPrefix("Frame Index").c_str(), &Frmidx, 0, (int)vecFrm.size() - 1))
                pCurAnim->m_CurFrmIdx = Frmidx;

            // Frame Infomation
            ImGui::Text("Frame Infomation");
            static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                           ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_SizingFixedSame;
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
            ImGui_InputText("Atlas Texture", ToString(pTex->GetKey()));
            ImGui::Image((void*)pSRV, ImVec2(TexSize.x, TexSize.y));
        }

        ImGui::Separator();

        if (ImGui_AlignButton("Load Animation", 0.f))
        {
            std::filesystem::path filePath = OpenFileDialog(L"", TEXT("애니메이션 파일\0*.anim\0모든 파일(*.*)\0*.*\0"));

            if (!filePath.empty()) // 취소, 닫기 버튼 체크
            {
                pAnimator->LoadAnimation(filePath.lexically_relative(CPathMgr::GetContentPath()));
            }
        }

        ImGui::SameLine();

        if (ImGui_AlignButton("Sprite Editor", 1.f))
        {
            CEditorMgr::GetInst()->GetLevelEditor()->ShowEditor(EDITOR_TYPE::SPRITE, true);
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

    bool open = ImGui::TreeNodeEx((void*)typeid(CLight2D).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::LIGHT2D]);

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

    bool open = ImGui::TreeNodeEx((void*)typeid(CLight3D).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::LIGHT3D]);

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
        if (ImGui::DragFloat(ImGui_LabelPrefix("Radius").c_str(), &fRadius, 1.f, 0.0f, D3D11_FLOAT32_MAX))
            pLight->SetRadius(fRadius);

        float fangle = pLight->GetAngle();
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

        int shadowType = pLight->GetShadowType();
        ImGui::Text("Shadow Type");
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(50.f, 0.f));
        ImGui::SameLine();
        ImGui::RadioButton("Static Shadow", &shadowType, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Dynamic Shadow", &shadowType, 1);
        pLight->SetShadowType(shadowType);

        float HaloRadius = pLight->GetHaloRadius();
        if (ImGui::DragFloat(ImGui_LabelPrefix("Halo Radius").c_str(), &HaloRadius, 1.f, 0.0f, D3D11_FLOAT32_MAX))
            pLight->SetHaloRadius(HaloRadius);

        float HaloStrength = pLight->GetHaloStrength();
        if (ImGui::SliderFloat(ImGui_LabelPrefix("Halo Strength").c_str(), &HaloStrength, 0.f, 1.f))
            pLight->SetHaloStrength(HaloStrength);

        int ShadowIdx = pLight->GetShadowIdx();
        if (ImGui::InputInt(ImGui_LabelPrefix("Shadow Index").c_str(), &ShadowIdx, 1, 100, ImGuiInputTextFlags_ReadOnly))
            pLight->SetShadowIdx(ShadowIdx);

        Ptr<CTexture> pDepthMapTex = pLight->GetDepthMapTex();
        void* TextureID = nullptr;

        if (nullptr != pDepthMapTex)
            TextureID = pDepthMapTex->GetSRV().Get();
        else
            TextureID = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\missing_texture.png", L"Texture\\missing_texture.png")->GetSRV().Get();

        ImGui::Text("ShadowMap Texture");
        ImGui::Image(TextureID, ImVec2(256.f, 256.f));

        ImGui::TreePop();
    }
}

void COutliner::DrawCamera(CGameObject* obj)
{
    CCamera* pCam = obj->Camera();
    if (nullptr == pCam)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CCamera).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::CAMERA]);

    ComponentSettingsButton(pCam);

    if (open)
    {
        const char* projectionTypeStrings[] = {"Orthographic", "Perspective"};
        const char* currentProjectionTypeString = projectionTypeStrings[(int)pCam->GetProjType()];
        if (ImGui::BeginCombo(ImGui_LabelPrefix("Projection").c_str(), currentProjectionTypeString))
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
            if (ImGui::DragFloat(ImGui_LabelPrefix("FOV").c_str(), &Degree, XM_PI / 18.f)) // 스피드 : 10도
                pCam->SetFOV(XMConvertToRadians(Degree));
        }

        if (pCam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
        {
            float scale = pCam->GetScale();
            if (ImGui::DragFloat(ImGui_LabelPrefix("Scale").c_str(), &scale, 0.01f, 1e-5f, 1000.f))
                pCam->SetScale(scale);
        }

        // Near Far
        float Near = pCam->GetNear();
        float Far = pCam->GetFar();
        float offset = 1.f;

        if (ImGui::DragFloat(ImGui_LabelPrefix("Near").c_str(), &Near, 1.f, 1.f, Far - offset))
            pCam->SetNear(Near);

        if (ImGui::DragFloat(ImGui_LabelPrefix("Far").c_str(), &Far, 1.f, Near + offset, 10000.f))
            pCam->SetFar(Far);

        // Layer Check
        UINT LayerMask = pCam->GetLayerMask();
        if (ImGui::BeginCombo(ImGui_LabelPrefix("Layer Mask").c_str(), "Select"))
        {
            CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

            for (int i = 0; i < LAYER_MAX; i++)
            {
                bool bCheck = LayerMask & (1 << i);

                if (ImGui::Checkbox(ToString(pCurLevel->GetLayer(i)->GetName()).c_str(), &bCheck))
                {
                    pCam->LayerMask(i, bCheck);
                }
            }

            ImGui::EndCombo();
        }

        // priority
        int priority = pCam->GetCameraPriority();
        if (ImGui::InputInt(ImGui_LabelPrefix("Camera Pritority").c_str(), &priority))
            pCam->SetCameraPriority(priority);

        // HDRI
        bool bHDRI = pCam->IsHDRI();
        if (ImGui::Checkbox(ImGui_LabelPrefix("HDRI").c_str(), &bHDRI))
            pCam->SetHDRI(bHDRI);

        ImGui::TreePop();
    }
}

void COutliner::DrawRigidbody2D(CGameObject* obj)
{
    CRigidbody2D* pRigidbody = obj->Rigidbody2D();
    if (nullptr == pRigidbody)
        return;

    bool open =
        ImGui::TreeNodeEx((void*)typeid(CRigidbody2D).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::RIGIDBODY2D]);

    ComponentSettingsButton(pRigidbody);

    if (open)
    {
        // =======================
        // Type
        // =======================
        vector<string> bodyTypes = {
            "Dynamic",
            "Kinematic",
            "Static",
        };

        static string CurbodyType = bodyTypes[(UINT)pRigidbody->GetBodyType()];

        CurbodyType = bodyTypes[(UINT)pRigidbody->GetBodyType()];

        if (ImGui_ComboUI(ImGui_LabelPrefix("Body Type").c_str(), CurbodyType, bodyTypes))
        {
            if (bodyTypes[0] == CurbodyType)
                pRigidbody->SetBodyType(BODY_TYPE::Dynamic);
            else if (bodyTypes[1] == CurbodyType)
                pRigidbody->SetBodyType(BODY_TYPE::Kinematic);
            else if (bodyTypes[2] == CurbodyType)
                pRigidbody->SetBodyType(BODY_TYPE::Static);
        }

        bool bSimulated = pRigidbody->IsSimulated();
        if (ImGui::Checkbox(ImGui_LabelPrefix("Simulated").c_str(), &bSimulated))
            pRigidbody->SetSimulated(bSimulated);

        bool bAutoMass = pRigidbody->IsUseAutoMass();
        if (ImGui::Checkbox(ImGui_LabelPrefix("Use Auto Mass").c_str(), &bAutoMass))
            pRigidbody->SetAutoMass(bAutoMass);

        float Mass = pRigidbody->GetMass();
        if (ImGui::DragFloat(ImGui_LabelPrefix("Mass").c_str(), &Mass, 0.1f))
            pRigidbody->SetMass(Mass);

        float LinearDrag = pRigidbody->GetLinearDrag();
        if (ImGui::DragFloat(ImGui_LabelPrefix("Linear Drag").c_str(), &LinearDrag, 0.1f))
            pRigidbody->SetLinearDrag(LinearDrag);

        float AngularDrag = pRigidbody->GetAngularDrag();
        if (ImGui::DragFloat(ImGui_LabelPrefix("Angular Drag").c_str(), &AngularDrag, 0.1f))
            pRigidbody->SetAngularDrag(AngularDrag);

        float GravityScale = pRigidbody->GetGravityScale();
        if (ImGui::DragFloat(ImGui_LabelPrefix("Gravity Scale").c_str(), &GravityScale, 0.1f))
            pRigidbody->SetGravityScale(GravityScale);

        bool bFreezeRotation = pRigidbody->IsFreezeRotation();
        if (ImGui::Checkbox(ImGui_LabelPrefix("Freeze Rotation Z").c_str(), &bFreezeRotation))
            pRigidbody->SetFreezeRotation(bFreezeRotation);

        ImGui::TreePop();
    }
}

void COutliner::DrawBoxCollider2D(CGameObject* obj)
{
    CBoxCollider2D* pBoxCol = obj->BoxCollider2D();
    if (nullptr == pBoxCol)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CBoxCollider2D).hash_code(), m_DefaultTreeNodeFlag,
                                  COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::BOXCOLLIDER2D]);

    ComponentSettingsButton(pBoxCol);

    if (open)
    {
        // Physics2D Material
        string MtrlName = string();
        Ptr<CPhysics2DMaterial> pMtrl = pBoxCol->GetMaterial();

        if (nullptr != pMtrl)
            MtrlName = ToString(pMtrl->GetName());

        ImGui_InputText("Material", MtrlName);

        // Drag & Drop
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_ASSETS"))
            {
                string name = (char*)payload->Data;
                name.resize(payload->DataSize);
                pBoxCol->SetMaterial(CAssetMgr::GetInst()->FindAsset<CPhysics2DMaterial>(ToWstring(name)));
            }

            ImGui::EndDragDropTarget();
        }

        bool bTrigger = pBoxCol->IsTrigger();
        if (ImGui::Checkbox(ImGui_LabelPrefix("Is Trigger").c_str(), &bTrigger))
            pBoxCol->SetTrigger(bTrigger);

        Vec2 Offset = pBoxCol->GetOffset();
        if (ImGui::DragFloat2(ImGui_LabelPrefix("Offset").c_str(), &Offset.x))
            pBoxCol->SetOffset(Offset);

        Vec2 Size = pBoxCol->GetSize();
        if (ImGui::DragFloat2(ImGui_LabelPrefix("Size").c_str(), &Size.x, 1e-3f, 1e-3f, D3D11_FLOAT32_MAX))
            pBoxCol->SetSize(Size);

        ImGui::Separator();

        if (ImGui_AlignButton("Physics2D Material Editor", 1.f))
        {
            CEditorMgr::GetInst()->GetLevelEditor()->ShowEditor(EDITOR_TYPE::PHYSICS2D_MATERIAL, true);
            CEditorMgr::GetInst()->GetPhysics2DMaterialEditor()->SetMaterial(pMtrl);
        }

        ImGui::TreePop();
    }
}

void COutliner::DrawCircleCollider2D(CGameObject* obj)
{
    CCircleCollider2D* pCircleCol = obj->CircleCollider2D();
    if (nullptr == pCircleCol)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CCircleCollider2D).hash_code(), m_DefaultTreeNodeFlag,
                                  COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::CIRCLECOLLIDER2D]);

    ComponentSettingsButton(pCircleCol);

    if (open)
    {
        // Physics2D Material
        string MtrlName = string();
        Ptr<CPhysics2DMaterial> pMtrl = pCircleCol->GetMaterial();

        if (nullptr != pMtrl)
            MtrlName = ToString(pMtrl->GetName());

        ImGui_InputText("Material", MtrlName);

        // Drag & Drop
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LEVEL_EDITOR_ASSETS"))
            {
                string name = (char*)payload->Data;
                name.resize(payload->DataSize);
                pCircleCol->SetMaterial(CAssetMgr::GetInst()->FindAsset<CPhysics2DMaterial>(ToWstring(name)));
            }

            ImGui::EndDragDropTarget();
        }

        bool bTrigger = pCircleCol->IsTrigger();
        if (ImGui::Checkbox(ImGui_LabelPrefix("Is Trigger").c_str(), &bTrigger))
            pCircleCol->SetTrigger(bTrigger);

        Vec2 Offset = pCircleCol->GetOffset();
        if (ImGui::DragFloat2(ImGui_LabelPrefix("Offset").c_str(), &Offset.x))
            pCircleCol->SetOffset(Offset);

        float Radius = pCircleCol->GetRadius();
        if (ImGui::DragFloat(ImGui_LabelPrefix("Radius").c_str(), &Radius, 1e-3f, 1e-3f, D3D11_FLOAT32_MAX))
            pCircleCol->SetRadius(Radius);

        ImGui::Separator();

        if (ImGui_AlignButton("Physics2D Material Editor", 1.f))
        {
            CEditorMgr::GetInst()->GetLevelEditor()->ShowEditor(EDITOR_TYPE::PHYSICS2D_MATERIAL, true);
            CEditorMgr::GetInst()->GetPhysics2DMaterialEditor()->SetMaterial(pMtrl);
        }

        ImGui::TreePop();
    }
}

void COutliner::DrawMeshRender(CGameObject* obj)
{
    // MeshRender
    CMeshRender* pMeshRender = obj->MeshRender();
    if (nullptr == pMeshRender)
        return;

    bool open =
        ImGui::TreeNodeEx((void*)typeid(CMeshRender).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::MESHRENDER]);

    ComponentSettingsButton(pMeshRender);

    if (open)
    {
        Ptr<CMesh> pMesh = pMeshRender->GetMesh();

        // Mesh
        if (ImGui::TreeNodeEx((void*)typeid(CMesh).hash_code(), m_DefaultTreeNodeFlag, "Mesh"))
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

        Ptr<CMaterial> pCurMtrl = pMeshRender->GetMaterial();
        Ptr<CMaterial> pSharedMtrl = pMeshRender->GetSharedMaterial();
        Ptr<CMaterial> pDynamicMtrl = pMeshRender->GetDynamicMaterial();

        // Material
        if (ImGui::TreeNodeEx((void*)typeid(CMaterial).hash_code(), m_DefaultTreeNodeFlag, "Material"))
        {
            string CurMtrlname, SharedMtrlname, DynamicMtrlname;
            if (nullptr != pCurMtrl)
                CurMtrlname = ToString(pCurMtrl->GetName());
            if (nullptr != pSharedMtrl)
                SharedMtrlname = ToString(pSharedMtrl->GetName());
            if (nullptr != pDynamicMtrl)
                DynamicMtrlname = ToString(pDynamicMtrl->GetName());

            ImGui_InputText("Material", CurMtrlname);

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

            ImGui_InputText("Shared Material", SharedMtrlname);
            ImGui_InputText("Dynamic Material", DynamicMtrlname);

            ImGui::Separator();

            if (ImGui_AlignButton("Material Editor", 1.f))
            {
                CEditorMgr::GetInst()->GetLevelEditor()->ShowEditor(EDITOR_TYPE::MATERIAL, true);
                CEditorMgr::GetInst()->GetMaterialEditor()->SetMaterial(pCurMtrl);
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

    bool open = ImGui::TreeNodeEx((void*)typeid(CTileMap).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::TILEMAP]);

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
        if (ImGui::TreeNodeEx("TileMap Info", m_DefaultTreeNodeFlag, "TileMap Info"))
        {
            int TileCountX = pTilemap->GetTileCountX();
            int TileCountY = pTilemap->GetTileCountY();
            Vec2 TileRenderSize = pTilemap->GetTileRenderSize();
            Ptr<CTexture> pTileAtlas = pTilemap->GetTileAtlas();
            Vec2 TilePixelSize = pTilemap->GetTilePixelSize();

            ImGui::InputInt(ImGui_LabelPrefix("Tile Count X").c_str(), &TileCountX, 1, 100, ImGuiInputTextFlags_ReadOnly);
            ImGui::InputInt(ImGui_LabelPrefix("Tile Count Y").c_str(), &TileCountY, 1, 100, ImGuiInputTextFlags_ReadOnly);
            ImGui::DragFloat2(ImGui_LabelPrefix("Tile Render Size").c_str(), &TileRenderSize.x, 0.f, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

            if (nullptr != pTileAtlas)
            {
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                string name = ToString(pTileAtlas->GetKey());
                strcpy_s(buffer, sizeof(buffer), name.c_str());
                ImGui::InputText(ImGui_LabelPrefix("Tile Atlas Name").c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly);

                ImGui::DragFloat2(ImGui_LabelPrefix("Tile Pixel Size").c_str(), &TilePixelSize.x, 0.f, 0.f, 0.f, "%.3f",
                                  ImGuiInputTextFlags_ReadOnly);
            }

            ImGui::TreePop();
        }

        ImGui::Separator();

        if (ImGui_AlignButton("TileMap Editor", 1.f))
        {
            CEditorMgr::GetInst()->GetLevelEditor()->ShowEditor(EDITOR_TYPE::TILEMAP, true);
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

    bool open = ImGui::TreeNodeEx((void*)typeid(CParticleSystem).hash_code(), m_DefaultTreeNodeFlag,
                                  COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);

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
        if (ImGui::TreeNodeEx((void*)typeid(tParticleModule).hash_code(), m_DefaultTreeNodeFlag, "Particle Module"))
        {
            // ============================================
            // Max Count
            // ============================================
            int MaxParticleCount = pParticleSystem->m_MaxParticleCount;
            if (ImGui::InputInt(ImGui_LabelPrefix("Max Particle Count").c_str(), &MaxParticleCount, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (MaxParticleCount < 0)
                    MaxParticleCount = 0;

                // 파티클 최대 갯수 제한
                if (MaxParticleCount > 1'000'000)
                    MaxParticleCount = 1'000'000;

                // 크기가 더 커진 경우 버퍼 재생성
                if ((int)pParticleSystem->m_MaxParticleCount < MaxParticleCount)
                {
                    pParticleSystem->m_ParticleBuffer->Create(sizeof(tParticle), MaxParticleCount, SB_TYPE::READ_WRITE, true);
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
            if (ImGui::RadioButton("Calculate Force", Module.arrModuleCheck[(UINT)PARTICLE_MODULE::CALCULATE_FORCE] == 1))
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
                    ImGui::Dummy(ImVec2(70.f, 0.f));
                    ImGui::SameLine();
                    ImGui::RadioButton("Local Space", (int*)&Module.SpaceType, 0);
                    ImGui::SameLine();
                    ImGui::RadioButton("World Space", (int*)&Module.SpaceType, 1);

                    ImGui::ColorEdit4(ImGui_LabelPrefix("Color").c_str(), &Module.vSpawnColor.x);
                    ImGui::DragFloat3(ImGui_LabelPrefix("Min Scale").c_str(), &Module.vSpawnMinScale.x, 1.f, 0.f, D3D11_FLOAT32_MAX);
                    ImGui::DragFloat3(ImGui_LabelPrefix("Max Scale").c_str(), &Module.vSpawnMaxScale.x, 1.f, 0.f, D3D11_FLOAT32_MAX);

                    ImGui::DragFloat(ImGui_LabelPrefix("Min Life").c_str(), &Module.MinLife, 0.1f, 0.f, Module.MaxLife);
                    ImGui::DragFloat(ImGui_LabelPrefix("Max Life").c_str(), &Module.MaxLife, 0.1f, Module.MinLife, D3D11_FLOAT32_MAX);
                    ImGui::DragFloat(ImGui_LabelPrefix("Min Mass").c_str(), &Module.MinMass, 0.1f, 0.f, Module.MaxMass);
                    ImGui::DragFloat(ImGui_LabelPrefix("Max Mass").c_str(), &Module.MaxMass, 0.1f, Module.MinMass, D3D11_FLOAT32_MAX);

                    if (ImGui::InputInt(ImGui_LabelPrefix("Spawn Rate").c_str(), &Module.SpawnRate, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        if (Module.SpawnRate < 0)
                            Module.SpawnRate = 0;
                    }

                    ImGui::Separator();

                    ImGui::Text("Spawn Shape");
                    ImGui::SameLine();
                    ImGui::Dummy(ImVec2(50.f, 0.f));
                    ImGui::SameLine();
                    ImGui::RadioButton("Sphere", (int*)&Module.SpawnShape, 0);
                    ImGui::SameLine();
                    ImGui::RadioButton("Box", (int*)&Module.SpawnShape, 1);

                    // Spawn Shape - Sphere
                    if (0 == Module.SpawnShape)
                    {
                        ImGui::DragFloat(ImGui_LabelPrefix("Radius").c_str(), &Module.Radius, 1.f, 0.f, D3D11_FLOAT32_MAX);
                    }
                    // Spawn Shape - Box
                    else if (1 == Module.SpawnShape)
                    {
                        ImGui::DragFloat3(ImGui_LabelPrefix("Spawn Box Scale").c_str(), &Module.vSpawnBoxScale.x, 1.f, 0.f, D3D11_FLOAT32_MAX);
                    }

                    ImGui::TreePop();
                }
            }

            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG])
            {
                if (ImGui::TreeNodeEx("Drag Module##Particlesystem", m_DefaultTreeNodeFlag, "Drag Module"))
                {
                    ImGui::DragFloat(ImGui_LabelPrefix("Drag Time").c_str(), &Module.DragTime, 1.f, 0.f, D3D11_FLOAT32_MAX);
                    ImGui::TreePop();
                }
            }

            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::SCALE])
            {
                if (ImGui::TreeNodeEx("Scale Module##Particlesystem", m_DefaultTreeNodeFlag, "Scale Module"))
                {
                    ImGui::DragFloat3(ImGui_LabelPrefix("Scale Ratio").c_str(), &Module.vScaleRatio.x, 1.f, 0.f, D3D11_FLOAT32_MAX);
                    ImGui::TreePop();
                }
            }

            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY])
            {
                if (ImGui::TreeNodeEx("Add Velocity Module##Particlesystem", m_DefaultTreeNodeFlag, "Add Velocity Module"))
                {
                    ImGui::Text("Velocity Type");
                    ImGui::SameLine();
                    ImGui::RadioButton("From Center", (int*)&Module.AddVelocityType, 0);
                    ImGui::SameLine();
                    ImGui::RadioButton("To Center", (int*)&Module.AddVelocityType, 1);
                    ImGui::SameLine();
                    ImGui::RadioButton("Fix Direction", (int*)&Module.AddVelocityType, 2);

                    ImGui::DragFloat(ImGui_LabelPrefix("Min Speed").c_str(), &Module.MinSpeed, 0.1f, 0.f, Module.MaxSpeed);
                    ImGui::DragFloat(ImGui_LabelPrefix("Max Speed").c_str(), &Module.MaxSpeed, 0.1f, Module.MinSpeed, D3D11_FLOAT32_MAX);

                    if (2 == Module.AddVelocityType)
                    {
                        ImGui::DragFloat(ImGui_LabelPrefix("Fixed Angle").c_str(), &Module.FixedAngle, 1.f, 0.f, 180.f);
                        ImGui::DragFloat3(ImGui_LabelPrefix("Fixed Direction").c_str(), &Module.vFixedDirection.x, 0.1f, -1.f, 1.f);
                    }

                    ImGui::TreePop();
                }
            }

            if (Module.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE])
            {
                if (ImGui::TreeNodeEx("Noise Force##Particlesystem", m_DefaultTreeNodeFlag, "Noise Force"))
                {
                    ImGui::DragFloat(ImGui_LabelPrefix("Noise Force Scale").c_str(), &Module.NoiseForceScale, 1.f, 0.f, D3D11_FLOAT32_MAX);
                    ImGui::DragFloat(ImGui_LabelPrefix("Noise Force Term").c_str(), &Module.NoiseForceTerm, 0.1f, 0.f, D3D11_FLOAT32_MAX);
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
                        ImGui::DragFloat(ImGui_LabelPrefix("Alpha Max Age").c_str(), &Module.AlphaMaxAge, 0.1f, 0.f, Module.MaxLife);
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
                TextureID = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\missing_texture.png", L"Texture\\missing_texture.png")->GetSRV().Get();

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
    CSkyBox* pSkyBox = obj->SkyBox();
    if (nullptr == pSkyBox)
        return;

    bool open = ImGui::TreeNodeEx((void*)typeid(CSkyBox).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::SKYBOX]);

    ComponentSettingsButton(pSkyBox);

    if (open)
    {
        // =======================
        // Type
        // =======================
        vector<string> SkyBoxTypes = {
            "IBLBaker",
            "LearnOpenGL",
            "moonless",
            "PureSky",
        };

        static string CurType = SkyBoxTypes[(UINT)pSkyBox->GetSkyBoxType()];

        CurType = SkyBoxTypes[(UINT)pSkyBox->GetSkyBoxType()];

        if (ImGui_ComboUI(ImGui_LabelPrefix("SkyBox Type").c_str(), CurType, SkyBoxTypes))
        {
            if (SkyBoxTypes[0] == CurType)
                pSkyBox->SetType(SKYBOX_TYPE::IBLBaker);
            else if (SkyBoxTypes[1] == CurType)
                pSkyBox->SetType(SKYBOX_TYPE::LearnOpenGL);
            else if (SkyBoxTypes[2] == CurType)
                pSkyBox->SetType(SKYBOX_TYPE::moonless);
            else if (SkyBoxTypes[3] == CurType)
                pSkyBox->SetType(SKYBOX_TYPE::PureSky);
        }

        // =======================
        // Shape
        // =======================
        vector<string> SkyBoxShapes = {
            "Sphere",
            "Box",
        };

        static string CurShape = SkyBoxShapes[(UINT)pSkyBox->GetSkyBoxShape()];
        if (ImGui_ComboUI(ImGui_LabelPrefix("SkyBox Shape").c_str(), CurShape, SkyBoxShapes))
        {
            if (SkyBoxShapes[0] == CurShape)
                pSkyBox->SetShape(SKYBOX_SHAPE::SPHERE);
            else if (SkyBoxShapes[1] == CurShape)
                pSkyBox->SetShape(SKYBOX_SHAPE::BOX);
        }

        ImGui::TreePop();
    }
}

void COutliner::DrawLandscape(CGameObject* obj)
{
}

void COutliner::DrawTextRender(CGameObject* obj)
{
    CTextRender* pTextRender = obj->TextRender();
    if (nullptr == pTextRender)
        return;

    bool open =
        ImGui::TreeNodeEx((void*)typeid(CTextRender).hash_code(), m_DefaultTreeNodeFlag, COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::TEXTRENDER]);

    ComponentSettingsButton(pTextRender);

    if (open)
    {
        wstring Str = pTextRender->GetText();
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), ToString(Str).c_str());
        if (ImGui::InputText(ImGui_LabelPrefix("Text").c_str(), buffer, sizeof(buffer)))
        {
            pTextRender->SetText(ToWstring(buffer));
        }

        Vec2 pos = pTextRender->GetTextPosition();
        if (ImGui::DragFloat2(ImGui_LabelPrefix("Position").c_str(), &pos.x))
            pTextRender->SetTextPosition(pos);

        float size = pTextRender->GetTextSize();
        if (ImGui::DragFloat(ImGui_LabelPrefix("Size").c_str(), &size))
            pTextRender->SetTextSize(size);

        Vec4 color = pTextRender->GetTextColor();
        color /= 255.f;
        if (ImGui::ColorEdit4(ImGui_LabelPrefix("Color").c_str(), &color.x))
        {
            color *= 255.f;
            pTextRender->SetTextColor(color);
        }

        ImGui::TreePop();
    }
}

void COutliner::DrawScript(CGameObject* obj)
{
    const vector<CScript*>& vecScript = obj->GetScripts();
    if (vecScript.empty())
        return;

    for (size_t i = 0; i < vecScript.size(); i++)
    {
        bool open = ImGui::TreeNodeEx((void*)(typeid(CScript).hash_code() + vecScript[i]->GetID()), m_DefaultTreeNodeFlag, "Script");

        ScriptSettingsButton(vecScript[i]);

        if (open)
        {
            ImGui_InputText("Name", ToString(CScriptMgr::GetScriptName(vecScript[i])));

            const vector<tScriptParam>& ScriptParams = vecScript[i]->GetScritpParam();

            for (int i = 0; i < ScriptParams.size(); i++)
            {
                switch (ScriptParams[i].eParam)
                {
                case SCRIPT_PARAM::INT:
                    ImGui::DragInt(ImGui_LabelPrefix(ScriptParams[i].strDesc.c_str()).c_str(), (int*)ScriptParams[i].pData);
                    break;
                case SCRIPT_PARAM::FLOAT:
                    ImGui::DragFloat(ImGui_LabelPrefix(ScriptParams[i].strDesc.c_str()).c_str(), (float*)ScriptParams[i].pData);
                    break;
                case SCRIPT_PARAM::VEC2:
                    ImGui::DragFloat2(ImGui_LabelPrefix(ScriptParams[i].strDesc.c_str()).c_str(), (float*)ScriptParams[i].pData);
                    break;
                case SCRIPT_PARAM::VEC3:
                    ImGui::DragFloat3(ImGui_LabelPrefix(ScriptParams[i].strDesc.c_str()).c_str(), (float*)ScriptParams[i].pData);
                    break;
                case SCRIPT_PARAM::VEC4:
                    ImGui::DragFloat4(ImGui_LabelPrefix(ScriptParams[i].strDesc.c_str()).c_str(), (float*)ScriptParams[i].pData);
                    break;
                case SCRIPT_PARAM::OBJECT: {
                    CGameObject* pObj = (CGameObject*)ScriptParams[i].pData;
                    string ObjName = string();
                    if (nullptr != pObj)
                        ObjName = ToString(pObj->GetName());

                    ImGui_InputText(ScriptParams[i].strDesc.c_str(), ObjName);
                }
                break;
                case SCRIPT_PARAM::STRING: {
                    string* pString = (string*)ScriptParams[i].pData;

                    char buffer[256];
                    memset(buffer, 0, sizeof(buffer));
                    strcpy_s(buffer, sizeof(buffer), (*pString).c_str());
                    if (ImGui::InputText(ImGui_LabelPrefix(ScriptParams[i].strDesc.c_str()).c_str(), buffer, sizeof(buffer)))
                    {
                        *pString = buffer;
                    }
                }
                break;
                }
            }

            ImGui::TreePop();
        }
    }
}

void COutliner::ScriptSettingsButton(CScript* script)
{
    ImVec2 contentRegion = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::SameLine(contentRegion.x - lineHeight * 0.5f);
    string id = "+";
    id += "##";
    id += typeid(CScript).name();
    id += script->GetID();
    if (ImGui::Button(id.c_str(), ImVec2{lineHeight, lineHeight}))
    {
        ImGui::OpenPopup(typeid(CScript).name() + script->GetID());
    }

    if (ImGui::BeginPopup(typeid(CScript).name() + script->GetID()))
    {
        if (ImGui::MenuItem("Remove Script"))
        {
            GamePlayStatic::RemoveScript(script->GetOwner(), script);
        }

        ImGui::EndPopup();
    }
}
