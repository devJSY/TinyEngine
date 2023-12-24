#include "pch.h"
#include "CDevice.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"

#include "CLevel.h"

#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CEngine.h"
#include "CTaskMgr.h"

#include "CTexture.h"

#include "CLight3D.h"

CLevelMgr::CLevelMgr()
    : m_CurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
    if (nullptr != m_CurLevel)
        delete m_CurLevel;
}

void CLevelMgr::init()
{
    m_CurLevel = new CLevel;
    m_CurLevel->begin();
    m_CurLevel->SetName(L"Test Level");

    // Camera Object 생성
    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);
    pCamObj->AddComponent(new CCameraMoveScript);

    pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -100.f));
    pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

    m_CurLevel->AddObject(pCamObj, 0);

    // Light
    CGameObject* pLight = new CGameObject;
    pLight->SetName(L"Light");

    pLight->AddComponent(new CTransform);
    pLight->AddComponent(new CMeshRender);
    pLight->AddComponent(new CLight3D(LIGHT_TYPE::SPOT));

    pLight->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
    pLight->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

    pLight->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pLight->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

    m_CurLevel->AddObject(pLight, 1);

    // GameObject 생성
    CGameObject* pObj = nullptr;

    pObj = new CGameObject;
    pObj->SetName(L"Player");

    pObj->AddComponent(new CTransform);
    pObj->AddComponent(new CMeshRender);
    pObj->AddComponent(new CPlayerScript);

    pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
    pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    
    pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pObj->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RedMaterial"));

    CGameObject* pChildObj = new CGameObject;
    pChildObj->SetName(L"Player Child");

    pChildObj->AddComponent(new CTransform);
    pChildObj->AddComponent(new CMeshRender);

    pChildObj->Transform()->SetRelativePos(Vec3(600.f, 0.f, -250.f));
    pChildObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pChildObj->Transform()->SetAbsolute(true);

    pChildObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pChildObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pChildObj->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pChildObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"GreenMaterial"));

    pObj->AddChild(pChildObj);

    m_CurLevel->AddObject(pObj, 10);

    // Material 테스트
    CGameObject* MatObj = new CGameObject;
    MatObj->SetName(L"Blue Material Object");

    MatObj->AddComponent(new CTransform);
    MatObj->AddComponent(new CMeshRender);

    MatObj->Transform()->SetRelativePos(Vec3(-600.f, 0.f, 750.f));
    MatObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

    MatObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CylinderMesh"));
    MatObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    MatObj->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    MatObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlueMaterial"));

    m_CurLevel->AddObject(MatObj, 10);
}

void CLevelMgr::tick()
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
            FTask task;
            task.Type = TASK_TYPE::WINDOW_RESIZE;
            task.Param_1 = (INT_PTR)width;
            task.Param_2 = (INT_PTR)height;

            CTaskMgr::GetInst()->AddTask(task);
        }
    }

    if (nullptr == m_CurLevel)
        return;

    m_CurLevel->tick();
    m_CurLevel->finaltick();
}

void CLevelMgr::render()
{
    if (nullptr == m_CurLevel)
        return;

    m_CurLevel->render();
}
