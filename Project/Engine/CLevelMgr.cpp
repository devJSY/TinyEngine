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

    pLight->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pLight->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pLight->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"YellowMaterial"));

    m_CurLevel->AddObject(pLight, 1);

    // Meshes
    CGameObject* pMeshes = new CGameObject;
    pMeshes->SetName(L"Meshes");

    pMeshes->AddComponent(new CTransform);
    pMeshes->AddComponent(new CMeshRender);
    pMeshes->AddComponent(new CPlayerScript);

    pMeshes->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pMeshes->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

    pMeshes->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pMeshes->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pMeshes->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlueMaterial"));

    // Square
    CGameObject* pSquare = new CGameObject;
    pSquare->SetName(L"Square");

    pSquare->AddComponent(new CTransform);
    pSquare->AddComponent(new CMeshRender);
    pSquare->AddComponent(new CPlayerScript);

    pSquare->Transform()->SetRelativePos(Vec3(-1000.f, 0.f, 500.f));
    pSquare->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pSquare->Transform()->SetAbsolute(true);

    pSquare->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SquareMesh"));
    pSquare->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pSquare->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pSquare->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pSquare);

    // SquareGrid
    CGameObject* pSquareGrid = new CGameObject;
    pSquareGrid->SetName(L"SquareGrid");

    pSquareGrid->AddComponent(new CTransform);
    pSquareGrid->AddComponent(new CMeshRender);
    pSquareGrid->AddComponent(new CPlayerScript);

    pSquareGrid->Transform()->SetRelativePos(Vec3(-750.f, 0.f, 500.f));
    pSquareGrid->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pSquareGrid->Transform()->SetAbsolute(true);

    pSquareGrid->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SquareGridMesh"));
    pSquareGrid->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pSquareGrid->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pSquareGrid->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pSquareGrid);

    // Box
    CGameObject* pBox = new CGameObject;
    pBox->SetName(L"Box");

    pBox->AddComponent(new CTransform);
    pBox->AddComponent(new CMeshRender);
    pBox->AddComponent(new CPlayerScript);

    pBox->Transform()->SetRelativePos(Vec3(-500.f, 0.f, 500.f));
    pBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pBox->Transform()->SetAbsolute(true);

    pBox->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    pBox->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pBox->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pBox->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pBox);

    // Cylinder
    CGameObject* pCylinder = new CGameObject;
    pCylinder->SetName(L"Cylinder");

    pCylinder->AddComponent(new CTransform);
    pCylinder->AddComponent(new CMeshRender);
    pCylinder->AddComponent(new CPlayerScript);

    pCylinder->Transform()->SetRelativePos(Vec3(-250.f, 0.f, 500.f));
    pCylinder->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pCylinder->Transform()->SetAbsolute(true);

    pCylinder->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CylinderMesh"));
    pCylinder->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pCylinder->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pCylinder->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pCylinder);

    // Sphere
    CGameObject* pSphere = new CGameObject;
    pSphere->SetName(L"Sphere");

    pSphere->AddComponent(new CTransform);
    pSphere->AddComponent(new CMeshRender);
    pSphere->AddComponent(new CPlayerScript);

    pSphere->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
    pSphere->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pSphere->Transform()->SetAbsolute(true);

    pSphere->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pSphere->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pSphere->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pSphere->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pSphere);

    // Tetrahedron
    CGameObject* pTetrahedron = new CGameObject;
    pTetrahedron->SetName(L"Tetrahedron");

    pTetrahedron->AddComponent(new CTransform);
    pTetrahedron->AddComponent(new CMeshRender);
    pTetrahedron->AddComponent(new CPlayerScript);

    pTetrahedron->Transform()->SetRelativePos(Vec3(250.f, 0.f, 500.f));
    pTetrahedron->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pTetrahedron->Transform()->SetAbsolute(true);

    pTetrahedron->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"TetrahedronMesh"));
    pTetrahedron->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pTetrahedron->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pTetrahedron->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pTetrahedron);

    // Icosahedron
    CGameObject* pIcosahedron = new CGameObject;
    pIcosahedron->SetName(L"Icosahedron");

    pIcosahedron->AddComponent(new CTransform);
    pIcosahedron->AddComponent(new CMeshRender);
    pIcosahedron->AddComponent(new CPlayerScript);

    pIcosahedron->Transform()->SetRelativePos(Vec3(500.f, 0.f, 500.f));
    pIcosahedron->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pIcosahedron->Transform()->SetAbsolute(true);

    pIcosahedron->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"IcosahedronMesh"));
    pIcosahedron->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pIcosahedron->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pIcosahedron->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pIcosahedron);

    // Subdivide SphereMesh
    CGameObject* pSubdivideSphere = new CGameObject;
    pSubdivideSphere->SetName(L"Subdivide SphereMesh");

    pSubdivideSphere->AddComponent(new CTransform);
    pSubdivideSphere->AddComponent(new CMeshRender);
    pSubdivideSphere->AddComponent(new CPlayerScript);

    pSubdivideSphere->Transform()->SetRelativePos(Vec3(750.f, 0.f, 500.f));
    pSubdivideSphere->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pSubdivideSphere->Transform()->SetAbsolute(true);

    pSubdivideSphere->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SubdivideSphereMesh"));
    pSubdivideSphere->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pSubdivideSphere->MeshRender()->SetNormalLineShader(
        CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pSubdivideSphere->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pSubdivideSphere);

    // Circle SphereMesh
    CGameObject* pCircle = new CGameObject;
    pCircle->SetName(L"Circle");

    pCircle->AddComponent(new CTransform);
    pCircle->AddComponent(new CMeshRender);
    pCircle->AddComponent(new CPlayerScript);

    pCircle->Transform()->SetRelativePos(Vec3(1000.f, 0.f, 500.f));
    pCircle->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pCircle->Transform()->SetAbsolute(true);

    pCircle->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
    pCircle->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pCircle->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pCircle->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pCircle);

    m_CurLevel->AddObject(pMeshes, 10);

    // Circle SphereMesh
    CGameObject* pZelda = new CGameObject;
    pZelda->SetName(L"Zelda");

    pZelda->AddComponent(new CTransform);
    pZelda->AddComponent(new CMeshRender);
    pZelda->AddComponent(new CPlayerScript);

    pZelda->Transform()->SetRelativePos(Vec3(0.f, 250.f, 500.f));
    pZelda->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pZelda->Transform()->SetAbsolute(true);

    pZelda->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pZelda->MeshRender()->SetNormalLineShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
    pZelda->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    for (auto& mesh : CAssetMgr::GetInst()->FindModel(L"Zelda"))
    {
        pZelda->MeshRender()->AddMesh(mesh);
    }

    m_CurLevel->AddObject(pZelda, 10);
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
