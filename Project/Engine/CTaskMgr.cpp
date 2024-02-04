#include "pch.h"

#include "CTaskMgr.h"
#include "CLevelMgr.h"
#include "CEditorMgr.h"
#include "CRenderMgr.h"
#include "CAssetMgr.h"
#include "CKeyMgr.h"
#include "CCollisionMgr.h"

#include "CEngine.h"
#include "CDevice.h"

#include "CGameObject.h"
#include "CComponent.h"
#include "components.h"

#include "CLevel.h"

#include "COutputLog.h"

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
        // 윈도우 사이즈 체크
        RECT rect;
        if (GetClientRect(CEngine::GetInst()->GetMainWind(), &rect))
        {
            Vec2 Resolution = CEngine::GetInst()->GetResolution();
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            if (Resolution.x != width || Resolution.y != height)
            {
                GamePlayStatic::WindowResize(width, height);
            }
        }

        // 스크린샷
        if (KEY_TAP(KEY::PRINT))
        {
            GamePlayStatic::ScreenShot();
        }

        // Mouse Picking
        if (KEY_TAP(KEY::LBTN))
        {
            // Color Picking
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
                        // GamePlayStatic::MouseRayPicking(MousePos); // Ray Picking
                    }
                }
                else
                {
                    GamePlayStatic::MouseColorPicking(MousePos); // Color Picking
                    // GamePlayStatic::MouseRayPicking(MousePos); // Ray Picking
                }
            }
        }

        // Destroy Object
        CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();
        if (nullptr != pSelectedObj && KEY_TAP(KEY::DEL))
        {
            GamePlayStatic::DestroyGameObject(pSelectedObj);
        }
    }

    for (size_t i = 0; i < m_vecTask.size(); ++i)
    {
        switch (m_vecTask[i].Type)
        {
        case TASK_TYPE::CREATE_OBJECT:
            CREATE_OBJECT(m_vecTask[i]);
            break;
        case TASK_TYPE::DELETE_OBJECT:
            DELETE_OBJECT(m_vecTask[i]);
            break;
        case TASK_TYPE::LEVEL_CHANGE:
            LEVEL_CHANGE(m_vecTask[i]);
            break;
        case TASK_TYPE::ADD_CHILD:
            ADD_CHILD(m_vecTask[i]);
            break;
        case TASK_TYPE::DISCONNECT_PARENT:
            DISCONNECT_PARENT(m_vecTask[i]);
            break;
        case TASK_TYPE::WINDOW_RESIZE:
            WINDOW_RESIZE(m_vecTask[i]);
            break;
        case TASK_TYPE::DELETE_ASSET:
            DELETE_ASSET(m_vecTask[i]);
            break;
        case TASK_TYPE::SCREENSHOT:
            SCREENSHOT(m_vecTask[i]);
            break;
        case TASK_TYPE::MOUSE_COLOR_PICKING:
            MOUSE_COLOR_PICKING(m_vecTask[i]);
            break;
        case TASK_TYPE::MOUSE_RAY_PICKING:
            MOUSE_RAY_PICKING(m_vecTask[i]);
            break;
        case TASK_TYPE::ADD_COMPONENT:
            ADD_COMPONENT(m_vecTask[i]);
            break;
        case TASK_TYPE::REMOVE_COMPONENT:
            REMOVE_COMPONENT(m_vecTask[i]);
            break;
        case TASK_TYPE::LAYER_CHANGE:
            LAYER_CHANGE(m_vecTask[i]);
            break;
        }
    }

    m_vecTask.clear();
}

void CTaskMgr::CREATE_OBJECT(const FTask& _Task)
{
    int LayerIdx = (int)_Task.Param_1;
    CGameObject* Object = (CGameObject*)_Task.Param_2;

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    pCurLevel->AddObject(Object, LayerIdx, true);
    CEditorMgr::GetInst()->SetSelectedObject(Object);

    /*if (LEVEL_STATE::PLAY == pCurLevel->GetState())
    {
        Object->begin();
    }*/
}

void CTaskMgr::DELETE_OBJECT(const FTask& _Task)
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

        for (size_t i = 0; i < pObject->m_vecChild.size(); ++i)
        {
            queue.push_back(pObject->m_vecChild[i]);
        }
    }

    // Selected Obj 해제
    if (pDeadObj == CEditorMgr::GetInst()->GetSelectedObject())
        CEditorMgr::GetInst()->SetSelectedObject(nullptr);
}

void CTaskMgr::LEVEL_CHANGE(const FTask& _Task)
{
    // Editor 초기화
    CEditorMgr::GetInst()->SetSelectedObject(nullptr);
    CEditorMgr::GetInst()->GetTileMapEditor()->SetTileMap(nullptr);

    CLevel* Level = (CLevel*)_Task.Param_1;
    CLevelMgr::GetInst()->ChangeLevel(Level);
    CRenderMgr::GetInst()->ClearCamera();
    Level->begin();
}

void CTaskMgr::ADD_CHILD(const FTask& _Task)
{
    CGameObject* pDestObj = (CGameObject*)_Task.Param_1;
    CGameObject* pSrcObj = (CGameObject*)_Task.Param_2;

    // 부모로 지정된 오브젝트가 없으면, Child 오브젝트가 최상위 부모 오브젝트가 된다.
    if (nullptr == pDestObj)
    {
        if (pSrcObj->GetParent())
        {
            // 기존 부모와의 연결 해제
            pSrcObj->DisconnectWithParent();

            // 최상위 부모 오브젝트로, 소속 레이어에 등록
            CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            pCurLevel->AddObject(pSrcObj, pSrcObj->m_iLayerIdx, true);
        }
    }
    else
    {
        // 레이어가 설정되지않은 자식이라면 부모의 레이어로 설정
        if (-1 == pSrcObj->m_iLayerIdx)
            pSrcObj->m_iLayerIdx = pDestObj->m_iLayerIdx;

        pDestObj->AddChild(pSrcObj);
    }
}

void CTaskMgr::DISCONNECT_PARENT(const FTask& _Task)
{
}

void CTaskMgr::WINDOW_RESIZE(const FTask& _Task)
{
    UINT width = (UINT)_Task.Param_1;
    UINT height = (UINT)_Task.Param_2;
    if (width <= 0 || height <= 0)
        return;

    Vec2 resolution = Vec2(width, height);
    CEngine::GetInst()->SetResolution(resolution);

    // 즉시 삭제
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"RenderTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"DepthStencilTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"RTCopyTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"IDMapTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"IDMapDSTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"FloatRenderTargetTexture");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"PostProessTex");

    CDevice::GetInst()->Resize(resolution);
    CRenderMgr::GetInst()->Resize(resolution);

    Ptr<CMaterial> pToneMappingMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ToneMappingMtrl");
    pToneMappingMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"FloatRenderTargetTexture"));

    LOG(Log, "Window Resized!");
}

void CTaskMgr::DELETE_ASSET(const FTask& _Task)
{
    ASSET_TYPE type = (ASSET_TYPE)_Task.Param_1;
    CAsset* pAsset = (CAsset*)_Task.Param_2;
    CAssetMgr::GetInst()->DeleteAsset(type, pAsset->GetKey());
}

void CTaskMgr::SCREENSHOT(const FTask& _Task)
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

void CTaskMgr::MOUSE_COLOR_PICKING(const FTask& _Task)
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
        for (size_t i = 0; i < vecObjects.size(); ++i)
        {
            // 오브젝트 이름으로 HashID 설정
            hash<wstring> hasher;
            int HashID = (int)hasher(vecObjects[i]->GetName());
            Vec4 colorID = HashIDToColor(HashID);

            // 0 ~ 1 → 0 ~ 255 범위확장
            uint8_t colorIDInt[4] = {static_cast<uint8_t>(colorID[0] * 255.f), static_cast<uint8_t>(colorID[1] * 255.f),
                                     static_cast<uint8_t>(colorID[2] * 255.f),
                                     static_cast<uint8_t>(colorID[3] * 255.f)};

            // Picking Color 비교
            if (m_pickColor[0] == colorIDInt[0] && m_pickColor[1] == colorIDInt[1] && m_pickColor[2] == colorIDInt[2] &&
                m_pickColor[3] == colorIDInt[3])
            {
                pSelectedObj = vecObjects[i];
                break;
            }
        }

        // 선택된 오브젝트가 있다면 탈출
        if (nullptr != pSelectedObj)
            break;
    }

    CEditorMgr::GetInst()->SetSelectedObject(pSelectedObj);
}

void CTaskMgr::MOUSE_RAY_PICKING(const FTask& _Task)
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

    Vector3 cursorNdcNear = Vector3(NdcMouseX, NdcMouseY, 0);
    Vector3 cursorNdcFar = Vector3(NdcMouseX, NdcMouseY, 1);

    // 메인 카메라
    CCamera* pCam = CRenderMgr::GetInst()->GetCamera(0);

    Matrix inverseProjView = (pCam->GetViewMat() * pCam->GetProjMat()).Invert();

    // ViewFrustum 안에서 PickingRay의 방향 구하기
    Vector3 NearWorld = Vector3::Transform(cursorNdcNear, inverseProjView);
    Vector3 FarWorld = Vector3::Transform(cursorNdcFar, inverseProjView);

    Vector3 dir = FarWorld - NearWorld;
    dir.Normalize();

    CGameObject* pSelectedObj = nullptr;

    // 광선을 만들고 충돌 감지
    SimpleMath::Ray curRay = SimpleMath::Ray(NearWorld, dir);
    float dist = 0.0f;

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        CLayer* pLayer = pCurLevel->GetLayer(i);
        const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
        for (size_t i = 0; i < vecObjects.size(); ++i)
        {
            float dist = 0.0f;
            CCollider2D* col = vecObjects[i]->Collider2D();
            if (nullptr == col)
                continue;

            bool bSelected = curRay.Intersects(col->GetBoundingSphere(), dist);

            if (bSelected)
            {
                pSelectedObj = vecObjects[i];
                break;
            }
        }

        // 선택된 오브젝트가 있다면 탈출
        if (nullptr != pSelectedObj)
            break;
    }

    CEditorMgr::GetInst()->SetSelectedObject(pSelectedObj);
}

void CTaskMgr::ADD_COMPONENT(const FTask& _Task)
{
    CGameObject* pObj = (CGameObject*)_Task.Param_1;
    COMPONENT_TYPE type = (COMPONENT_TYPE)_Task.Param_2;

    CComponent* pCom = pObj->GetComponent(type);
    // 이미 해당 컴포넌트를 보유한 경우
    if (nullptr != pCom)
        return;

    switch (type)
    {
    case COMPONENT_TYPE::TRANSFORM:
        pObj->AddComponent(new CTransform);
        break;
    case COMPONENT_TYPE::COLLIDER2D:
        pObj->AddComponent(new CCollider2D);
        break;
    case COMPONENT_TYPE::COLLIDER3D:
        break;
    case COMPONENT_TYPE::ANIMATOR2D:
        pObj->AddComponent(new CAnimator2D);
        break;
    case COMPONENT_TYPE::ANIMATOR3D:
        break;
    case COMPONENT_TYPE::LIGHT2D:
        pObj->AddComponent(new CLight2D());
        break;
    case COMPONENT_TYPE::LIGHT3D:
        pObj->AddComponent(new CLight3D());
        break;
    case COMPONENT_TYPE::CAMERA:
        pObj->AddComponent(new CCamera);
        break;
    case COMPONENT_TYPE::MESHRENDER:
        {
            if (nullptr == pObj->GetRenderComponent())
                pObj->AddComponent(new CMeshRender);
            else
                LOG(Error, "%s %s", ToString(pObj->GetName()).c_str(), "Already Has a RenderComponent!!");
        }
        break;
    case COMPONENT_TYPE::TILEMAP:
        {
            if (nullptr == pObj->GetRenderComponent())
                pObj->AddComponent(new CTileMap);
            else
                LOG(Error, "%s %s", ToString(pObj->GetName()).c_str(), "Already Has a RenderComponent!!");
        }
        break;
    case COMPONENT_TYPE::PARTICLESYSTEM:
        {
            if (nullptr == pObj->GetRenderComponent())
                pObj->AddComponent(new CParticleSystem);
            else
                LOG(Error, "%s %s", ToString(pObj->GetName()).c_str(), "Already Has a RenderComponent!!");
        }
        break;
    case COMPONENT_TYPE::SKYBOX:
        break;
    case COMPONENT_TYPE::DECAL:
        break;
    case COMPONENT_TYPE::LANDSCAPE:
        break;
    }
}

void CTaskMgr::REMOVE_COMPONENT(const FTask& _Task)
{
    CGameObject* pObj = (CGameObject*)_Task.Param_1;
    COMPONENT_TYPE type = (COMPONENT_TYPE)_Task.Param_2;
    pObj->RemoveComponent(type);
}

void CTaskMgr::LAYER_CHANGE(const FTask& _Task)
{
    CGameObject* Object = (CGameObject*)_Task.Param_1;
    int NextLayerIdx = (int)_Task.Param_2;
    int OriginLayerIdx = Object->GetLayerIdx();

    CCollisionMgr::GetInst()->CollisionRelease(Object);
    CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(OriginLayerIdx)->DetachGameObject(Object);
    CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(Object, NextLayerIdx, false);
}
