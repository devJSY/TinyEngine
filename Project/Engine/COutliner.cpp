#include "pch.h"
#include "COutliner.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CRenderMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "components.h"

#include "CTexture.h"
#include "CAnim.h"
#include "CMaterial.h"

COutliner::COutliner()
    : m_DefaultTreeNodeFlag(ImGuiTreeNodeFlags_None)
{
}

COutliner::~COutliner()
{
}

void COutliner::DrawNode(CGameObject* obj)
{
    CGameObject* pSelectedObj = CLevelMgr::GetInst()->GetSelectedObject();

    int id = -1;

    if (nullptr != pSelectedObj)
        id = pSelectedObj->GetID();

    ImGuiTreeNodeFlags flags =
        ((id == obj->GetID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

    std::string name = WstringTostring(obj->GetName());

    bool opened = ImGui::TreeNodeEx((void*)(intptr_t)obj->GetID(), flags, name.c_str());

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        CLevelMgr::GetInst()->SetSelectedObject(obj);
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

static std::string _labelPrefix(const char* const label)
{
    float width = ImGui::CalcItemWidth();

    float x = ImGui::GetCursorPosX();
    ImGui::Text(label);
    ImGui::SameLine();
    ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::SetNextItemWidth(-1);

    std::string labelID = "##";
    labelID += label;

    return labelID;
}

//
// Combo with std::vector<std::string>
//
static bool ImGuiComboUI(const std::string& caption, std::string& current_item, const std::vector<std::string>& items)
{
    bool changed = false;

    if (ImGui::BeginCombo(caption.c_str(), current_item.c_str()))
    {
        for (int n = 0; n < items.size(); n++)
        {
            bool is_selected = (current_item == items[n]);
            if (ImGui::Selectable(items[n].c_str(), is_selected))
            {
                current_item = items[n];
                changed = true;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    return changed;
}

void COutliner::DrawDetails(CGameObject* obj)
{
    // Tag
    {
        std::string name = WstringTostring(obj->GetName());

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), name.c_str());
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            obj->SetName(stringToWstring(buffer));
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
            if (ImGui::MenuItem(GetComponentName((COMPONENT_TYPE)i).c_str()))
            {
                GamePlayStatic::AddComponent(CLevelMgr::GetInst()->GetSelectedObject(), (COMPONENT_TYPE)i);
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
            LayerNames.push_back(WstringTostring(pCurLevel->GetLayer(i)->GetName()));
        }

        int LayerIdx = obj->GetLayerIdx();
        std::string name = LayerNames[LayerIdx];

        if (ImGuiComboUI(_labelPrefix("Layer").c_str(), name, LayerNames))
        {
            GamePlayStatic::LayerChange(obj, pCurLevel->GetLayer(stringToWstring(name))->GetLayerIdx());
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

void COutliner::init()
{
    m_DefaultTreeNodeFlag = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
}

void COutliner::render()
{
    // 유효성체크
    // IsValid(m_SelectedObj);

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
        CLevelMgr::GetInst()->SetSelectedObject(nullptr);

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

    ImGui::End();

    ImGui::Begin("Details");
    if (nullptr != CLevelMgr::GetInst()->GetSelectedObject())
        DrawDetails(CLevelMgr::GetInst()->GetSelectedObject());

    ImGui::End();
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
        DrawVec3Control("Location", pos, 10.f);
        pTr->SetRelativePos(pos);

        Vec3 rot = pTr->GetRelativeRotation();
        DrawVec3Control("Rotation", rot, XM_PI / 180.f);
        pTr->SetRelativeRotation(rot);

        Vec3 scale = pTr->GetRelativeScale();
        DrawVec3Control("Scale", scale, 1.f, 1.f, D3D11_FLOAT32_MAX, 1.f);
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
        if (ImGui::BeginCombo(_labelPrefix("Collider2DType").c_str(), currentCollider2DTypeString))
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
            ImGui::Checkbox(_labelPrefix("Absolute").c_str(), &bAbsolute);
            pCol->SetAbsolute(bAbsolute);
        }
        else if (pCol->GetColliderType() == COLLIDER2D_TYPE::CIRCLE)
        {
            float fRadius = pCol->GetRadius();
            if (ImGui::DragFloat(_labelPrefix("Radius").c_str(), &fRadius, 1.f, 0.0f, D3D11_FLOAT32_MAX))
                pCol->SetRadius(fRadius);
        }

        Vec2 offsetPos = pCol->GetOffsetPos();
        ImGui::DragFloat2(_labelPrefix("Offset Pos").c_str(), &offsetPos.x, 0.1f);
        pCol->SetOffsetPos(offsetPos);

        Vec2 offsetScale = pCol->GetOffsetScale();
        ImGui::DragFloat2(_labelPrefix("Offset Scale").c_str(), &offsetScale.x, 0.1f, 0.f, D3D11_FLOAT32_MAX);
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
                names.push_back(WstringTostring(iter.first));
            }

            string curAnimName = WstringTostring(pCurAnim->GetName());

            ImGui::Text("Animation Name");
            if (ImGuiComboUI("##Anim", curAnimName, names))
            {
                pAni->Play(stringToWstring(curAnimName), true);
            }

            pCurAnim = pAni->GetCurAnim();

            Ptr<CTexture> pTex = pCurAnim->GetAtlasTex();
            Vec2 TexSize = Vec2((float)pTex->GetWidth(), (float)pTex->GetHeight());
            const vector<tAnimFrm>& vecFrm = pCurAnim->GetVecFrm();

            // Frame Index
            int Frmidx = pCurAnim->GetCurFrmIdx();
            ImGui::Text("Frame Index");
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::SliderInt("##FrmIdx", &Frmidx, 0, (int)vecFrm.size());
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
        if (ImGui::BeginCombo(_labelPrefix("Light Type").c_str(), currentLightTypeStrings))
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

        Vec4 color = pLight->GetLightColor();
        if (ImGui::ColorEdit3(_labelPrefix("Color").c_str(), &color.x))
            pLight->SetLightColor(color);

        Vec4 specular = pLight->GetSpecular();
        if (ImGui::ColorEdit3(_labelPrefix("Specular").c_str(), &specular.x))
            pLight->SetSpecular(specular);

        Vec4 ambient = pLight->GetAmbient();
        if (ImGui::ColorEdit3(_labelPrefix("Ambient").c_str(), &ambient.x))
            pLight->SetAmbient(ambient);

        float fRadius = pLight->GetRadius();
        float fangle = pLight->GetAngle();

        if (ImGui::DragFloat(_labelPrefix("Radius").c_str(), &fRadius, 1.f, 0.0f, D3D11_FLOAT32_MAX))
            pLight->SetRadius(fRadius);

        if (ImGui::SliderFloat(_labelPrefix("Angle").c_str(), &fangle, 0.0f, 180.f))
            pLight->SetAngle(fangle);

        float FallOffStart = pLight->GetFallOffStart();
        float FallOffEnd = pLight->GetFallOffEnd();
        float offset = 1.f;

        if (ImGui::SliderFloat(_labelPrefix("FallOffStart").c_str(), &FallOffStart, 0.0f, FallOffEnd - offset))
            pLight->SetFallOffStart(FallOffStart);

        if (ImGui::SliderFloat(_labelPrefix("FallOffEnd").c_str(), &FallOffEnd, FallOffStart + offset, 10000.f))
            pLight->SetFallOffEnd(FallOffEnd);

        float spotPower = pLight->GetSpotPower();
        if (ImGui::SliderFloat(_labelPrefix("Spot Power").c_str(), &spotPower, 1.f, 256.f))
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
        if (ImGui::BeginCombo(_labelPrefix("Light Type").c_str(), currentLightTypeStrings))
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

        Vec4 color = pLight->GetLightColor();
        if (ImGui::ColorEdit3(_labelPrefix("Color").c_str(), &color.x))
            pLight->SetLightColor(color);

        Vec4 specular = pLight->GetSpecular();
        if (ImGui::ColorEdit3(_labelPrefix("Specular").c_str(), &specular.x))
            pLight->SetSpecular(specular);

        Vec4 ambient = pLight->GetAmbient();
        if (ImGui::ColorEdit3(_labelPrefix("Ambient").c_str(), &ambient.x))
            pLight->SetAmbient(ambient);

        float fRadius = pLight->GetRadius();
        float fangle = pLight->GetAngle();

        if (ImGui::DragFloat(_labelPrefix("Radius").c_str(), &fRadius, 1.f, 0.0f, D3D11_FLOAT32_MAX))
            pLight->SetRadius(fRadius);

        if (ImGui::SliderFloat(_labelPrefix("Angle").c_str(), &fangle, 0.0f, XM_PI))
            pLight->SetAngle(fangle);

        float FallOffStart = pLight->GetFallOffStart();
        float FallOffEnd = pLight->GetFallOffEnd();
        float offset = 1.f;

        if (ImGui::SliderFloat(_labelPrefix("FallOffStart").c_str(), &FallOffStart, 0.0f, FallOffEnd - offset))
            pLight->SetFallOffStart(FallOffStart);

        if (ImGui::SliderFloat(_labelPrefix("FallOffEnd").c_str(), &FallOffEnd, FallOffStart + offset, 10000.f))
            pLight->SetFallOffEnd(FallOffEnd);

        float spotPower = pLight->GetSpotPower();
        if (ImGui::SliderFloat(_labelPrefix("Spot Power").c_str(), &spotPower, 1.f, 256.f))
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
        Ptr<CMaterial> pMaterial = pMeshRender->GetMaterial();
        Ptr<CMesh> pMesh = pMeshRender->GetMesh();

        // Mesh
        if (ImGui::TreeNodeEx((void*)typeid(CMesh).hash_code(), m_DefaultTreeNodeFlag, "Mesh"))
        {
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            if (nullptr != pMesh)
            {
                std::string name = WstringTostring(pMesh->GetName());
                strcpy_s(buffer, sizeof(buffer), name.c_str());
            }
            ImGui::InputText(_labelPrefix("Mesh").c_str(), buffer, sizeof(buffer));

            // Drag & Drop
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
                {
                    string name = (char*)payload->Data;
                    name.resize(payload->DataSize);
                    pMeshRender->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(stringToWstring(name)));
                }

                ImGui::EndDragDropTarget();
            }

            ImGui::TreePop();
        }

        // Material
        if (ImGui::TreeNodeEx((void*)typeid(CMaterial).hash_code(), m_DefaultTreeNodeFlag, "Material"))
        {
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            if (nullptr != pMaterial)
            {
                std::string name = WstringTostring(pMaterial->GetName());
                strcpy_s(buffer, sizeof(buffer), name.c_str());
            }
            ImGui::InputText(_labelPrefix("Material").c_str(), buffer, sizeof(buffer));

            // Drag & Drop
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
                {
                    string name = (char*)payload->Data;
                    name.resize(payload->DataSize);
                    pMeshRender->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(stringToWstring(name)));
                }

                ImGui::EndDragDropTarget();
            }

            if (nullptr != pMaterial)
            {
                const tMtrlConst& MtrlConst = pMaterial->GetMtrlConst();

                Vec4 ambient = MtrlConst.mtrl.vAmb;
                Vec4 diffuse = MtrlConst.mtrl.vDiff;
                Vec4 specular = MtrlConst.mtrl.vSpec;
                Vec4 environment = MtrlConst.mtrl.vEmv;

                bool bDirty = false;

                if (ImGui::ColorEdit3(_labelPrefix("Ambient").c_str(), &ambient.x))
                    bDirty = true;

                if (ImGui::ColorEdit3(_labelPrefix("diffuse").c_str(), &diffuse.x))
                    bDirty = true;

                if (ImGui::ColorEdit3(_labelPrefix("specular").c_str(), &specular.x))
                    bDirty = true;

                if (ImGui::ColorEdit3(_labelPrefix("environment").c_str(), &environment.x))
                    bDirty = true;

                if (bDirty)
                    pMaterial->SetMaterialCoefficient(ambient, diffuse, specular, environment);

                constexpr float IMAGE_BASE_SIZE = 250.0f;

                // Texture
                for (UINT i = TEX_PARAM::TEX_0; i <= TEX_PARAM::TEX_5; ++i)
                {
                    Ptr<CTexture> pTex = pMaterial->GetTexParam((TEX_PARAM)i);
                    ID3D11ShaderResourceView* pSRV = nullptr;

                    if (nullptr != pTex.Get())
                        pSRV = pTex->GetSRV().Get();
                    else
                        pSRV = CAssetMgr::GetInst()->FindAsset<CTexture>(L"missing_texture")->GetSRV().Get();

                    if (i == TEX_PARAM::TEX_0)
                        ImGui::Text("Texture 0");
                    else if (i == TEX_PARAM::TEX_1)
                        ImGui::Text("Texture 1");
                    else if (i == TEX_PARAM::TEX_2)
                        ImGui::Text("Texture 2");
                    else if (i == TEX_PARAM::TEX_3)
                        ImGui::Text("Texture 3");
                    else if (i == TEX_PARAM::TEX_4)
                        ImGui::Text("Texture 4");
                    else if (i == TEX_PARAM::TEX_5)
                        ImGui::Text("Texture 5");

                    ImGui::Image((void*)pSRV, ImVec2(IMAGE_BASE_SIZE, IMAGE_BASE_SIZE));

                    // Drag & Drop
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                        {
                            string name = (char*)payload->Data;
                            name.resize(payload->DataSize);
                            pMaterial->SetTexParam((TEX_PARAM)i,
                                                   CAssetMgr::GetInst()->FindAsset<CTexture>(stringToWstring(name)));
                        }

                        ImGui::EndDragDropTarget();
                    }
                }
            }

            ImGui::TreePop();
        }

        // Option
        if (ImGui::TreeNodeEx("##MeshRenderOption", m_DefaultTreeNodeFlag, "Option"))
        {
            bool bUseTexture = pMeshRender->IsUseTexture();
            if (ImGui::Checkbox("Use Texture", &bUseTexture))
                pMeshRender->SetUseTexture(bUseTexture);

            bool bNormalLine = pMeshRender->IsDrawNormalLine();
            if (ImGui::Checkbox("Draw NormalLine", &bNormalLine))
                pMeshRender->SetDrawNormalLine(bNormalLine);

            float scale = pMeshRender->GetNormalLineScale();
            if (ImGui::SliderFloat("NormalLineScale", &scale, 1.f, 100.f))
                pMeshRender->SetNormalLineScale(scale);

            bool bUseRim = pMeshRender->IsUseRim();
            if (ImGui::Checkbox("Use Rim", &bUseRim))
                pMeshRender->SetUseRim(bUseRim);

            Vec3 color = pMeshRender->GetRimColor();
            if (ImGui::ColorEdit3("Color", &color.x))
                pMeshRender->SetRimColor(color);

            float power = pMeshRender->GetRimPower();
            if (ImGui::SliderFloat("Power", &power, 0.f, 10.f))
                pMeshRender->SetRimPower(power);

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

void COutliner::DrawTileMap(CGameObject* obj)
{
}

void COutliner::DrawParticlesystem(CGameObject* obj)
{
}

void COutliner::DrawSkybox(CGameObject* obj)
{
}

void COutliner::DrawLandscape(CGameObject* obj)
{
}
