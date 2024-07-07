#include "pch.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"
#include "CEditorMgr.h"
#include "CRenderMgr.h"
#include "CAssetMgr.h"
#include "CKeyMgr.h"
#include "CPhysics2DMgr.h"
#include "CPhysicsMgr.h"
#include "CGrabageCollector.h"
#include <Scripts\\CScriptMgr.h>

#include "CEngine.h"
#include "CDevice.h"

#include "CGameObject.h"
#include "CComponent.h"
#include "components.h"

#include "CLevel.h"

#include <box2d\\b2_fixture.h>

#include "CLevelSaveLoad.h"

// stb
extern "C"
{
#define _CRT_SECURE_NO_WARNINGS // stb_image_write compile error fix
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
}

CTaskMgr::CTaskMgr()
{
}

CTaskMgr::~CTaskMgr()
{
}

void CTaskMgr::tick()
{
    {
        // Mouse Picking
        if (KEY_TAP(KEY::LBTN))
        {
            Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();

            // Editor 모드
            if (CEditorMgr::GetInst()->IsEnable())
            {
                // Viewport 에서만 마우스 피킹 적용
                // Viewport 기준 마우스위치로 설정
                if (CEditorMgr::GetInst()->GetLevelEditor()->IsViewportHovered())
                {
                    MousePos = CEditorMgr::GetInst()->GetViewportMousePos();

                    GamePlayStatic::MouseColorPicking(MousePos); // Color Picking
                    // GamePlayStatic::MouseCollision2DPicking(MousePos); // Collision Picking
                }
            }
            else
            {
                GamePlayStatic::MouseColorPicking(MousePos); // Color Picking
                // GamePlayStatic::MouseCollision2DPicking(MousePos); // Collision Picking
            }
        }

        if (CEditorMgr::GetInst()->IsEnable())
        {
            // 스크린샷
            if (KEY_TAP(KEY::PRINT))
            {
                GamePlayStatic::ScreenShot();
            }

            // 에셋 저장
            if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::S))
            {
                CAssetMgr::GetInst()->SaveAssetsToFile();
                LOG(Log, "Assets Saved to file Successfully!");
            }
        }
    }

    while (!m_queueTask.empty())
    {
        const tTask& Task = m_queueTask.front();
        m_queueTask.pop();

        switch (Task.Type)
        {
        case TASK_TYPE::CREATE_OBJECT:
            CREATE_OBJECT(Task);
            break;
        case TASK_TYPE::DELETE_OBJECT:
            DELETE_OBJECT(Task);
            break;
        case TASK_TYPE::CHANGE_LEVELSTATE:
            CHANGE_LEVELSTATE(Task);
            break;
        case TASK_TYPE::ADD_CHILD:
            ADD_CHILD(Task);
            break;
        case TASK_TYPE::WINDOW_RESIZE:
            WINDOW_RESIZE(Task);
            break;
        case TASK_TYPE::DELETE_ASSET:
            DELETE_ASSET(Task);
            break;
        case TASK_TYPE::SCREENSHOT:
            SCREENSHOT(Task);
            break;
        case TASK_TYPE::MOUSE_COLOR_PICKING:
            MOUSE_COLOR_PICKING(Task);
            break;
        case TASK_TYPE::MOUSE_COLLISION2D_PICKING:
            MOUSE_COLLISION2D_PICKING(Task);
            break;
        case TASK_TYPE::ADD_COMPONENT:
            ADD_COMPONENT(Task);
            break;
        case TASK_TYPE::REMOVE_COMPONENT:
            REMOVE_COMPONENT(Task);
            break;
        case TASK_TYPE::REMOVE_SCRIPT:
            REMOVE_SCRIPT(Task);
            break;
        case TASK_TYPE::CHANGE_LAYER:
            CHANGE_LAYER(Task);
            break;
        case TASK_TYPE::CLONE_OBJECT:
            CLONE_OBJECT(Task);
            break;
        case TASK_TYPE::PHYSICS2D_EVNET:
            PHYSICS2D_EVNET(Task);
            break;
        case TASK_TYPE::PHYSICS_EVNET:
            PHYSICS_EVNET(Task);
            break;
        case TASK_TYPE::CHANGE_LEVEL: {
            // 이벤트중에서 제일 마지막에 처리
            if (m_queueTask.empty())
                CHANGE_LEVEL(Task);
            else
                m_queueTask.push(Task);
        }
        break;
        }
    }
}

void CTaskMgr::CREATE_OBJECT(const tTask& _Task)
{
    int LayerIdx = (int)_Task.Param_1;
    CGameObject* pObject = (CGameObject*)_Task.Param_2;

    if (-1 == LayerIdx)
        LayerIdx = 0;

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    pCurLevel->AddObject(pObject, LayerIdx, false);
    CEditorMgr::GetInst()->SetSelectedObject(pObject);

    CPhysics2DMgr::GetInst()->AddPhysicsObject(pObject);
    CPhysicsMgr::GetInst()->AddPhysicsObject(pObject);
    for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
    {
        CPhysics2DMgr::GetInst()->AddPhysicsObject(pObject->m_vecChild[i]);
        CPhysicsMgr::GetInst()->AddPhysicsObject(pObject->m_vecChild[i]);
    }

    if (LEVEL_STATE::PLAY == pCurLevel->GetState() || LEVEL_STATE::SIMULATE == pCurLevel->GetState())
    {
        pObject->begin();
    }
}

void CTaskMgr::DELETE_OBJECT(const tTask& _Task)
{
    CGameObject* pDeadObj = (CGameObject*)_Task.Param_1;

    list<CGameObject*> queue;
    queue.push_back(pDeadObj);

    // 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 Dead 처리
    while (!queue.empty())
    {
        CGameObject* pObject = queue.front();
        queue.pop_front();

        pObject->m_bDead = true;
        CPhysics2DMgr::GetInst()->RemovePhysicsObject(pObject);
        CPhysicsMgr::GetInst()->RemovePhysicsObject(pObject);

        for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
        {
            queue.push_back(pObject->m_vecChild[i]);
        }
    }

    // Selected Obj 해제
    if (pDeadObj == CEditorMgr::GetInst()->GetSelectedObject())
        CEditorMgr::GetInst()->SetSelectedObject(nullptr);
}

void CTaskMgr::CHANGE_LEVELSTATE(const tTask& _Task)
{
    CLevel* pNexLevel = (CLevel*)_Task.Param_1;
    LEVEL_STATE NextState = (LEVEL_STATE)_Task.Param_2;

    pNexLevel->ChangeState(NextState);
}

void CTaskMgr::ADD_CHILD(const tTask& _Task)
{
    CGameObject* pDestObj = (CGameObject*)_Task.Param_1;
    CGameObject* pSrcObj = (CGameObject*)_Task.Param_2;
    tBoneSocket* BoneSocket = (tBoneSocket*)_Task.Param_3;

    // 부모 오브젝트가 자신의 자식오브젝트의 자식으로 들어가려는 경우는 방지
    if (pDestObj != nullptr && pDestObj->IsAncestor(pSrcObj))
        return;

    // 부모로 지정된 오브젝트가 없으면, Child 오브젝트가 최상위 부모 오브젝트가 된다.
    if (nullptr == pDestObj)
    {
        if (pSrcObj->GetParent())
        {
            pSrcObj->Transform()->SetRelativePos(pSrcObj->Transform()->GetWorldPos());
            pSrcObj->Transform()->SetRelativeRotation(pSrcObj->Transform()->GetWorldRotation());
            pSrcObj->Transform()->SetRelativeScale(pSrcObj->Transform()->GetWorldScale());

            // 기존 부모와의 연결 해제
            int layerIdx = pSrcObj->m_iLayerIdx;
            pSrcObj->DisconnectWithParent();

            // 최상위 부모 오브젝트로, 소속 레이어에 등록
            CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            pCurLevel->AddObject(pSrcObj, layerIdx, false);
        }
    }
    else
    {
        pSrcObj->Transform()->SetRelativePos(pSrcObj->Transform()->GetRelativePos() - pDestObj->Transform()->GetWorldPos());
        pSrcObj->Transform()->SetRelativeRotation(pSrcObj->Transform()->GetRelativeRotation() - pDestObj->Transform()->GetWorldRotation());

        pDestObj->AddChild(pSrcObj);
        pSrcObj->SetBoneSocket(BoneSocket);
    }
}

void CTaskMgr::WINDOW_RESIZE(const tTask& _Task)
{
    UINT width = (UINT)_Task.Param_1;
    UINT height = (UINT)_Task.Param_2;
    if (width <= 0 || height <= 0)
        return;

    Vec2 resolution = Vec2(width, height);
    CEngine::GetInst()->SetResolution(resolution);

    CRenderMgr::GetInst()->Resize_Release();
    CDevice::GetInst()->Resize(resolution);
    CRenderMgr::GetInst()->Resize(resolution);

    LOG(Log, "Window Resized!");
}

void CTaskMgr::DELETE_ASSET(const tTask& _Task)
{
    ASSET_TYPE type = (ASSET_TYPE)_Task.Param_1;
    CAsset* pAsset = (CAsset*)_Task.Param_2;
    CAssetMgr::GetInst()->DeleteAsset(type, pAsset->GetKey());
}

void CTaskMgr::SCREENSHOT(const tTask& _Task)
{
    Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
    Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");

    //// MSAA 사용시 Texture2DMS → Texture2D
    // Ptr<CTexture> tempTex = CAssetMgr::GetInst()->CreateTexture(
    //     L"tempTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
    //     D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
    //;
    // CONTEXT->ResolveSubresource(tempTex->GetTex2D().Get(), 0, pTex->GetTex2D().Get(), 0,
    //                             DXGI_FORMAT_R8G8B8A8_UNORM);

    // Texture2D → StagingTexture
    D3D11_TEXTURE2D_DESC desc;
    pTex->GetTex2D()->GetDesc(&desc);
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags = 0;
    desc.MiscFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU에서 읽기 가능
    desc.Usage = D3D11_USAGE_STAGING;            // GPU에서 CPU로 보낼 데이터를 임시 보관

    ComPtr<ID3D11Texture2D> stagingTexture;
    DEVICE->CreateTexture2D(&desc, nullptr, stagingTexture.GetAddressOf());

    // 전체 복사
    CONTEXT->CopyResource(stagingTexture.Get(), pTex->GetTex2D().Get());

    // GPU → CPU
    // R8G8B8A8 이라고 가정
    std::vector<uint8_t> pixels(desc.Width * desc.Height * 4);

    D3D11_MAPPED_SUBRESOURCE ms;
    CONTEXT->Map(stagingTexture.Get(), NULL, D3D11_MAP_READ, NULL,
                 &ms); // D3D11_MAP_READ 주의

    // 텍스춰가 작을 경우에는
    // ms.RowPitch가 width * sizeof(uint8_t) * 4보다 클 수도 있어서
    // for문으로 가로줄 하나씩 복사
    uint8_t* pData = (uint8_t*)ms.pData;
    for (unsigned int h = 0; h < desc.Height; h++)
    {
        memcpy(&pixels[h * desc.Width * 4], &pData[h * ms.RowPitch], desc.Width * sizeof(uint8_t) * 4);
    }

    CONTEXT->Unmap(stagingTexture.Get(), NULL);
    string filename = ToString(CPathMgr::GetOutputPath());
    filename += "screenshot\\";

    // Check if the folder exists, if not, create it
    if (!std::filesystem::exists(filename))
    {
        std::filesystem::create_directory(filename);
    }

    filename += currentDateTime();
    filename += ".png";
    if (stbi_write_png(filename.c_str(), desc.Width, desc.Height, 4, pixels.data(), desc.Width * 4))
    {
        LOG(Log, "%s %s", "Screenshot! FileName :", filename.c_str());
    }
}

void CTaskMgr::MOUSE_COLOR_PICKING(const tTask& _Task)
{
    if (CEditorMgr::GetInst()->IsEnable() && (ImGuizmo::IsOver() || ImGuizmo::IsUsing()))
        return;

    int MouseX = (int)_Task.Param_1;
    int MouseY = (int)_Task.Param_2;
    Vec2 WindowSize = CDevice::GetInst()->GetRenderResolution();

    if (CEditorMgr::GetInst()->IsEnable())
    {
        Vec2 ViewportSize = CEditorMgr::GetInst()->GetViewportSize();
        if (ViewportSize.x <= 0 || ViewportSize.y <= 0)
            return;

        // Viewport Screen → 0 ~ 1
        float NdcMouseX = MouseX / ViewportSize.x;
        float NdcMouseY = MouseY / ViewportSize.y;

        // 0 ~ 1 → Window Screen
        MouseX = int(NdcMouseX * WindowSize.x);
        MouseY = int(NdcMouseY * WindowSize.y);
    }

    if (MouseX < 0 || MouseY < 0 || MouseX >= WindowSize.x || MouseY >= WindowSize.y)
        return;

    Ptr<CTexture> pIDMapTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"IDMapTex");

    // Texture2D → StagingTexture
    D3D11_TEXTURE2D_DESC desc;
    pIDMapTex->GetTex2D()->GetDesc(&desc);
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags = 0;
    desc.MiscFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU에서 읽기 가능
    desc.Usage = D3D11_USAGE_STAGING;            // GPU에서 CPU로 보낼 데이터를 임시 보관

    ComPtr<ID3D11Texture2D> stagingTexture;
    DEVICE->CreateTexture2D(&desc, nullptr, stagingTexture.GetAddressOf());

    // 마우스위치의 1픽셀만 복사
    D3D11_BOX box;
    box.left = MouseX;
    box.right = MouseX + 1;
    box.top = MouseY;
    box.bottom = MouseY + 1;
    box.front = 0;
    box.back = 1;
    CONTEXT->CopySubresourceRegion(stagingTexture.Get(), 0, 0, 0, 0, pIDMapTex->GetTex2D().Get(), 0, &box);

    // R8G8B8A8
    D3D11_MAPPED_SUBRESOURCE ms;
    CONTEXT->Map(stagingTexture.Get(), NULL, D3D11_MAP_READ, NULL, &ms);
    uint8_t* pData = (uint8_t*)ms.pData;
    uint8_t m_pickColor[4] = {};
    memcpy(m_pickColor, pData, sizeof(uint8_t) * 4);
    CONTEXT->Unmap(stagingTexture.Get(), NULL);

    CGameObject* pSelectedObj = nullptr;

    // 현재 레벨의 오브젝트와 ID값 비교
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        CLayer* pLayer = pCurLevel->GetLayer(i);
        const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
        for (size_t j = 0; j < vecObjects.size(); ++j)
        {
            // 오브젝트 이름 + ID값으로 HashID Find
            hash<wstring> hasher;
            int HashID = (int)hasher(vecObjects[j]->GetName()) + vecObjects[j]->GetID();
            Vec4 colorID = HashIDToColor(HashID);

            // 0 ~ 1 → 0 ~ 255 범위확장
            uint8_t colorIDInt[4] = {static_cast<uint8_t>(colorID[0] * 255.f), static_cast<uint8_t>(colorID[1] * 255.f),
                                     static_cast<uint8_t>(colorID[2] * 255.f), static_cast<uint8_t>(colorID[3] * 255.f)};

            // Picking Color 비교
            if (m_pickColor[0] == colorIDInt[0] && m_pickColor[1] == colorIDInt[1] && m_pickColor[2] == colorIDInt[2] &&
                m_pickColor[3] == colorIDInt[3])
            {
                pSelectedObj = vecObjects[j];
                break;
            }
        }

        // 선택된 오브젝트가 있다면 탈출
        if (nullptr != pSelectedObj)
            break;
    }

    CEditorMgr::GetInst()->SetSelectedObject(pSelectedObj);
}
//
// void CTaskMgr::MOUSE_RAY_PICKING(const tTask& _Task)
//{
//    if (CEditorMgr::GetInst()->IsEnable() && (ImGuizmo::IsOver() || ImGuizmo::IsUsing()))
//        return;
//
//    int MouseX = (int)_Task.Param_1;
//    int MouseY = (int)_Task.Param_2;
//
//    float NdcMouseX = 0.0f;
//    float NdcMouseY = 0.0f;
//
//    // 마우스 커서의 위치를 NDC로 변환
//    // 마우스 커서는 좌측 상단 (0, 0), 우측 하단(width-1, height-1)
//    // NDC는 좌측 하단이 (-1, -1), 우측 상단(1, 1)
//    if (CEditorMgr::GetInst()->IsEnable())
//    {
//        Vec2 ViewportSize = CEditorMgr::GetInst()->GetViewportSize();
//        if (ViewportSize.x <= 0 || ViewportSize.y <= 0)
//            return;
//
//        NdcMouseX = MouseX * 2.0f / ViewportSize.x - 1.0f;
//        NdcMouseY = -MouseY * 2.0f / ViewportSize.y + 1.0f;
//    }
//    else
//    {
//        Vec2 WindowSize = CDevice::GetInst()->GetRenderResolution();
//        NdcMouseX = MouseX * 2.0f / WindowSize.x - 1.0f;
//        NdcMouseY = -MouseY * 2.0f / WindowSize.y + 1.0f;
//    }
//
//    if (NdcMouseX < -1.0 || NdcMouseY < -1.0 || NdcMouseX > 1.0 || NdcMouseY > 1.0)
//        return;
//
//    Vector3 cursorNdcNear = Vector3(NdcMouseX, NdcMouseY, 0);
//    Vector3 cursorNdcFar = Vector3(NdcMouseX, NdcMouseY, 1);
//
//    // 메인 카메라
//    CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();
//
//    Matrix inverseProjView = (pCam->GetViewMat() * pCam->GetProjMat()).Invert();
//
//    // ViewFrustum 안에서 PickingRay의 방향 구하기
//    Vector3 NearWorld = Vector3::Transform(cursorNdcNear, inverseProjView);
//    Vector3 FarWorld = Vector3::Transform(cursorNdcFar, inverseProjView);
//
//    Vector3 dir = FarWorld - NearWorld;
//    dir.Normalize();
//
//    CGameObject* pSelectedObj = nullptr;
//
//    // 광선을 만들고 충돌 감지
//    SimpleMath::Ray curRay = SimpleMath::Ray(NearWorld, dir);
//    float dist = 0.0f;
//
//    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
//    for (int i = 0; i < LAYER_MAX; ++i)
//    {
//        CLayer* pLayer = pCurLevel->GetLayer(i);
//        const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
//        for (size_t j = 0; j < vecObjects.size(); ++j)
//        {
//            float dist = 0.0f;
//            CCollider2D* col = vecObjects[j]->Collider2D();
//            if (nullptr == col)
//                continue;
//
//            bool bSelected = false;
//            if (COLLIDER2D_TYPE::RECT == col->GetColliderType())
//                bSelected = curRay.Intersects(col->GetBoundingBox(), dist);
//            else
//                bSelected = curRay.Intersects(col->GetBoundingSphere(), dist);
//
//            if (bSelected)
//            {
//                pSelectedObj = vecObjects[j];
//                break;
//            }
//        }
//
//        // 선택된 오브젝트가 있다면 탈출
//        if (nullptr != pSelectedObj)
//            break;
//    }
//
//    CEditorMgr::GetInst()->SetSelectedObject(pSelectedObj);
//}

void CTaskMgr::MOUSE_COLLISION2D_PICKING(const tTask& _Task)
{
    if (CEditorMgr::GetInst()->IsEnable() && (ImGuizmo::IsOver() || ImGuizmo::IsUsing()))
        return;

    int MouseX = (int)_Task.Param_1;
    int MouseY = (int)_Task.Param_2;

    float NdcMouseX = 0.0f;
    float NdcMouseY = 0.0f;

    // 마우스 커서의 위치를 NDC로 변환
    // 마우스 커서는 좌측 상단 (0, 0), 우측 하단(width-1, height-1)
    // NDC는 좌측 하단이 (-1, -1), 우측 상단(1, 1)
    if (CEditorMgr::GetInst()->IsEnable())
    {
        Vec2 ViewportSize = CEditorMgr::GetInst()->GetViewportSize();
        if (ViewportSize.x <= 0 || ViewportSize.y <= 0)
            return;

        NdcMouseX = MouseX * 2.0f / ViewportSize.x - 1.0f;
        NdcMouseY = -MouseY * 2.0f / ViewportSize.y + 1.0f;
    }
    else
    {
        Vec2 WindowSize = CDevice::GetInst()->GetRenderResolution();
        NdcMouseX = MouseX * 2.0f / WindowSize.x - 1.0f;
        NdcMouseY = -MouseY * 2.0f / WindowSize.y + 1.0f;
    }

    if (NdcMouseX < -1.0 || NdcMouseY < -1.0 || NdcMouseX > 1.0 || NdcMouseY > 1.0)
        return;

    CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();

    // Mouse Pos - NDC Near → World
    Vector3 cursorNdcNear = Vector3(NdcMouseX, NdcMouseY, 0);
    Matrix inverseProjView = pCam->GetProjInvMat() * pCam->GetViewInvMat();
    Vector3 NearWorld = Vector3::Transform(cursorNdcNear, inverseProjView);

    CGameObject* pSelectedObj = nullptr;

    pSelectedObj = CPhysics2DMgr::GetInst()->CollisionCheck(Vec2(NearWorld.x, NearWorld.y));

    CEditorMgr::GetInst()->SetSelectedObject(pSelectedObj);
}

void CTaskMgr::ADD_COMPONENT(const tTask& _Task)
{
    CGameObject* pObj = (CGameObject*)_Task.Param_1;
    COMPONENT_TYPE type = (COMPONENT_TYPE)_Task.Param_2;

    CComponent* pCom = pObj->GetComponent(type);
    // 이미 해당 컴포넌트를 보유한 경우
    if (nullptr != pCom)
    {
        LOG(Error, "%s Already Has a %s Component!!", ToString(pObj->GetName()).c_str(), COMPONENT_TYPE_STRING[(UINT)type]);
        return;
    }

    // RenderComponent 예외처리
    if (pObj->GetRenderComponent())
    {
        if (COMPONENT_TYPE::MESHRENDER == type || COMPONENT_TYPE::TILEMAP == type || COMPONENT_TYPE::PARTICLESYSTEM == type ||
            COMPONENT_TYPE::SKYBOX == type || COMPONENT_TYPE::DECAL == type || COMPONENT_TYPE::LANDSCAPE == type ||
            COMPONENT_TYPE::TEXTRENDER == type)
        {
            LOG(Error, "%s %s", ToString(pObj->GetName()).c_str(), "Already Has a RenderComponent!!");
            return;
        }
    }

    CPhysics2DMgr::GetInst()->RemovePhysicsObject(pObj);
    CPhysicsMgr::GetInst()->RemovePhysicsObject(pObj);

    switch (type)
    {
    case COMPONENT_TYPE::TRANSFORM:
        pObj->AddComponent(new CTransform);
        break;
    case COMPONENT_TYPE::ANIMATOR2D:
        pObj->AddComponent(new CAnimator2D);
        break;
    case COMPONENT_TYPE::ANIMATOR:
        pObj->AddComponent(new CAnimator);
        break;
    case COMPONENT_TYPE::LIGHT2D:
        pObj->AddComponent(new CLight2D);
        break;
    case COMPONENT_TYPE::LIGHT:
        pObj->AddComponent(new CLight);
        break;
    case COMPONENT_TYPE::CAMERA:
        pObj->AddComponent(new CCamera);
        break;
    case COMPONENT_TYPE::RIGIDBODY2D:
        pObj->AddComponent(new CRigidbody2D);
        break;
    case COMPONENT_TYPE::BOXCOLLIDER2D:
        pObj->AddComponent(new CBoxCollider2D);
        break;
    case COMPONENT_TYPE::CIRCLECOLLIDER2D:
        pObj->AddComponent(new CCircleCollider2D);
        break;
    case COMPONENT_TYPE::POLYGONCOLLIDER2D:
        pObj->AddComponent(new CPolygonCollider2D);
        break;
    case COMPONENT_TYPE::EDGECOLLIDER2D:
        pObj->AddComponent(new CEdgeCollider2D);
        break;
    case COMPONENT_TYPE::RIGIDBODY:
        pObj->AddComponent(new CRigidbody);
        break;
    case COMPONENT_TYPE::BOXCOLLIDER:
        pObj->AddComponent(new CBoxCollider);
        break;
    case COMPONENT_TYPE::SPHERECOLLIDER:
        pObj->AddComponent(new CSphereCollider);
        break;
    case COMPONENT_TYPE::CAPSULECOLLIDER:
        pObj->AddComponent(new CCapsuleCollider);
        break;
    case COMPONENT_TYPE::MESHCOLLIDER: {
        pObj->AddComponent(new CMeshCollider);
        if (nullptr != pObj->MeshRender())
        {
            pObj->MeshCollider()->SetMesh(pObj->MeshRender()->GetMesh());
        }
    }
    break;
    case COMPONENT_TYPE::CHARACTERCONTROLLER:
        pObj->AddComponent(new CCharacterController);
        break;
    case COMPONENT_TYPE::MESHRENDER:
        pObj->AddComponent(new CMeshRender);
        break;
    case COMPONENT_TYPE::TILEMAP:
        pObj->AddComponent(new CTileMap);
        break;
    case COMPONENT_TYPE::PARTICLESYSTEM:
        pObj->AddComponent(new CParticleSystem);
        break;
    case COMPONENT_TYPE::SKYBOX:
        pObj->AddComponent(new CSkyBox);
        break;
    case COMPONENT_TYPE::DECAL:
        pObj->AddComponent(new CDecal);
        break;
    case COMPONENT_TYPE::LANDSCAPE:
        pObj->AddComponent(new CLandScape);
        break;
    case COMPONENT_TYPE::TEXTRENDER:
        pObj->AddComponent(new CTextRender);
        break;
    }

    CPhysics2DMgr::GetInst()->AddPhysicsObject(pObj);
    CPhysicsMgr::GetInst()->AddPhysicsObject(pObj);
}

void CTaskMgr::REMOVE_COMPONENT(const tTask& _Task)
{
    CGameObject* pObj = (CGameObject*)_Task.Param_1;
    COMPONENT_TYPE type = (COMPONENT_TYPE)_Task.Param_2;

    CPhysics2DMgr::GetInst()->RemovePhysicsObject(pObj);
    CPhysicsMgr::GetInst()->RemovePhysicsObject(pObj);
    pObj->RemoveComponent(type);
    CPhysics2DMgr::GetInst()->AddPhysicsObject(pObj);
    CPhysicsMgr::GetInst()->AddPhysicsObject(pObj);
}

void CTaskMgr::REMOVE_SCRIPT(const tTask& _Task)
{
    CGameObject* pObj = (CGameObject*)_Task.Param_1;
    CScript* script = (CScript*)_Task.Param_2;
    pObj->RemoveScript(script);
}

void CTaskMgr::CHANGE_LAYER(const tTask& _Task)
{
    CGameObject* Object = (CGameObject*)_Task.Param_1;
    int NextLayerIdx = (int)_Task.Param_2;
    int OriginLayerIdx = Object->GetLayerIdx();

    GamePlayStatic::Physics2D_Event(Object, Physics2D_EVENT_TYPE::RESPAWN);
    GamePlayStatic::Physics_Event(Object, Physics_EVENT_TYPE::RESPAWN);
    CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(OriginLayerIdx)->DetachGameObject(Object);
    CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(Object, NextLayerIdx, false);
}

void CTaskMgr::CLONE_OBJECT(const tTask& _Task)
{
    CGameObject* OriginObject = (CGameObject*)_Task.Param_1;

    // 레이어에 속해있지않은 오브젝트는 복사하지 않음
    if (-1 == OriginObject->m_iLayerIdx)
        return;

    CGameObject* CloneObj = OriginObject->Clone();
    CloneObj->Transform()->SetRelativePos(OriginObject->Transform()->GetWorldPos());
    CloneObj->Transform()->SetRelativeRotation(OriginObject->Transform()->GetWorldRotation());
    CloneObj->Transform()->SetRelativeScale(OriginObject->Transform()->GetWorldScale());

    CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(CloneObj, CloneObj->m_iLayerIdx, false);
    CEditorMgr::GetInst()->SetSelectedObject(CloneObj);
}

void CTaskMgr::PHYSICS2D_EVNET(const tTask& _Task)
{
    CGameObject* pObj = (CGameObject*)_Task.Param_1;
    Physics2D_EVENT_TYPE type = (Physics2D_EVENT_TYPE)_Task.Param_2;

    switch (type)
    {
    case Physics2D_EVENT_TYPE::RESPAWN: {
        CPhysics2DMgr::GetInst()->RemovePhysicsObject(pObj);
        CPhysics2DMgr::GetInst()->AddPhysicsObject(pObj);
    }
    break;
    }
}

void CTaskMgr::PHYSICS_EVNET(const tTask& _Task)
{
    CGameObject* pObj = (CGameObject*)_Task.Param_1;
    Physics_EVENT_TYPE type = (Physics_EVENT_TYPE)_Task.Param_2;

    switch (type)
    {
    case Physics_EVENT_TYPE::RESPAWN: {
        CPhysicsMgr::GetInst()->RemovePhysicsObject(pObj);
        CPhysicsMgr::GetInst()->AddPhysicsObject(pObj);
    }
    break;
    }
}

void CTaskMgr::CHANGE_LEVEL(const tTask& _Task)
{
    // Editor 초기화
    CEditorMgr::GetInst()->SetSelectedObject(nullptr);
    if (CEditorMgr::GetInst()->IsEnable())
    {
        CEditorMgr::GetInst()->GetTileMapEditor()->SetTileMap(nullptr);
    }

    CLevel* pNexLevel = (CLevel*)_Task.Param_1;
    LEVEL_STATE NextState = (LEVEL_STATE)_Task.Param_2;

    CGrabageCollector::GetInst()->Clear();
    CRenderMgr::GetInst()->ClearCamera();
    CPhysics2DMgr::GetInst()->OnPhysics2DStop();
    CPhysicsMgr::GetInst()->OnPhysicsStop();
    CLevelMgr::GetInst()->ChangeLevel(pNexLevel, NextState);
}
