#include "pch.h"
#include "COutliner.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CRenderMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CMaterial.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CLight3D.h"
#include "CCamera.h"
#include "CMeshRender.h"

#include "CTexture.h"
#include "CCollider2D.h"

COutliner::COutliner()
{
}

COutliner::~COutliner()
{
}

void COutliner::DrawNode(CGameObject* obj)
{
    CGameObject* SelectedObj = CLevelMgr::GetInst()->GetSelectedObj();

    int id = -1;

    if (nullptr != SelectedObj)
        id = SelectedObj->GetID();

    ImGuiTreeNodeFlags flags =
        ((id == obj->GetID()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

    std::string name = WstringTostring(obj->GetName());

    bool opened = ImGui::TreeNodeEx((void*)(intptr_t)obj->GetID(), flags, name.c_str());

    if (ImGui::IsItemClicked())
    {
        CLevelMgr::GetInst()->SetSelectObj(obj);
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

void COutliner::DrawDetails(CGameObject* obj)
{
    // Tag
    {
        std::string name = WstringTostring(obj->GetName());

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), name.c_str());
        ImGui::InputText("##Tag", buffer, sizeof(buffer));
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
                GamePlayStatic::AddComponent(CLevelMgr::GetInst()->GetSelectedObj(), (COMPONENT_TYPE)i);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

    // Transform
    CTransform* pTr = obj->Transform();
    if (nullptr != pTr)
    {
        if (ImGui::TreeNodeEx((void*)typeid(CTransform).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
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

    // Collider2D
    CCollider2D* pCol = obj->Collider2D();
    if (nullptr != pCol)
    {
        if (ImGui::TreeNodeEx((void*)typeid(CCollider2D).hash_code(), ImGuiTreeNodeFlags_None, "Collider2D"))
        {
            const char* Collider2DTypeStrings[] = {"Rect", "Circle"};
            const char* currentCollider2DTypeString = Collider2DTypeStrings[(int)pCol->GetType()];
            if (ImGui::BeginCombo("Collider2DType", currentCollider2DTypeString))
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

            if (pCol->GetType() == COLLIDER2D_TYPE::RECT)
            {
                bool bAbsolute = pCol->IsAbsolute();
                ImGui::Checkbox("Absolute", &bAbsolute);
                pCol->SetAbsolute(bAbsolute);
            }
            else if (pCol->GetType() == COLLIDER2D_TYPE::CIRCLE)
            {
                float fRadius = pCol->GetRadius();
                if (ImGui::DragFloat("Radius", &fRadius, 1.f, 0.0f, 10000.f))
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

    // Camera
    CCamera* pCam = obj->Camera();
    if (nullptr != pCam)
    {
        if (ImGui::TreeNodeEx((void*)typeid(CCamera).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
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
                if (ImGui::DragFloat("Scale", &scale, 0.01f, 0.001f, 100.f))
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

    // Light3D
    CLight3D* pLight = obj->Light3D();
    if (nullptr != pLight)
    {
        if (ImGui::TreeNodeEx((void*)typeid(CLight3D).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Light3D"))
        {
            const char* LightTypeStrings[] = {"Directional Light", "Point Light", "Spot Light"};
            const char* currentLightTypeStrings = LightTypeStrings[(int)pLight->GetLightType()];
            if (ImGui::BeginCombo("Light Type", currentLightTypeStrings))
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

            float FallOffStart = pLight->GetFallOffStart();
            float FallOffEnd = pLight->GetFallOffEnd();
            float offset = 1.f;

            if (ImGui::SliderFloat("FallOffStart ", &FallOffStart, 0.0f, FallOffEnd - offset))
                pLight->SetFallOffStart(FallOffStart);

            if (ImGui::SliderFloat("FallOffEnd", &FallOffEnd, FallOffStart + offset, 10000.f))
                pLight->SetFallOffEnd(FallOffEnd);

            float spotPower = pLight->GetSpotPower();
            if (ImGui::SliderFloat("Spot Power", &spotPower, 1.f, 1000.f))
                pLight->SetSpotPower(spotPower);

            Vec3 color = pLight->GetColor();
            if (ImGui::ColorEdit3("Color", &color.x))
                pLight->SetColor(color);

            ImGui::TreePop();
        }
    }

    // MeshRender
    CMeshRender* pMeshRender = obj->MeshRender();
    if (nullptr != pMeshRender)
    {
        if (ImGui::TreeNodeEx((void*)typeid(CMeshRender).hash_code(), ImGuiTreeNodeFlags_None, "MeshRender"))
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

            ImGui::TreePop();
        }

        // Material
        Ptr<CMaterial> pMaterial = pMeshRender->GetMaterial();
        if (nullptr != pMaterial)
        {
            if (ImGui::TreeNodeEx((void*)typeid(CMaterial).hash_code(), ImGuiTreeNodeFlags_None, "Material"))
            {
                const tMtrlConst& MtrlConst = pMaterial->GetMtrlConst();

                Vec4 ambient = MtrlConst.mtrl.vAmb;
                Vec4 diffuse = MtrlConst.mtrl.vDiff;
                Vec4 specular = MtrlConst.mtrl.vSpec;
                Vec4 environment = MtrlConst.mtrl.vEmv;

                bool bDirty = false;

                if (ImGui::ColorEdit3(_labelPrefix("Ambient").c_str(), &ambient.x))
                    bDirty = true;

                if (ImGui::SliderFloat3(_labelPrefix("diffuse").c_str(), &diffuse.x, 0.f, 1.f))
                    bDirty = true;

                if (ImGui::SliderFloat3(_labelPrefix("specular").c_str(), &specular.x, 0.f, 1.f))
                    bDirty = true;

                if (ImGui::SliderFloat3(_labelPrefix("environment").c_str(), &environment.x, 0.f, 1.f))
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

                ImGui::TreePop();
            }
        }

        if (ImGui::TreeNodeEx("Rim", ImGuiTreeNodeFlags_None, "Rim Light"))
        {
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
    }
}

void COutliner::init()
{
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
        CLevelMgr::GetInst()->SetSelectObj(nullptr);

    // Right-click on blank space
    if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
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

            CLevelMgr::GetInst()->SetSelectObj(pObj);
            GamePlayStatic::SpawnGameObject(pObj, 0);
        }

        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Details");
    if (nullptr != CLevelMgr::GetInst()->GetSelectedObj())
        DrawDetails(CLevelMgr::GetInst()->GetSelectedObj());

    ImGui::End();
}