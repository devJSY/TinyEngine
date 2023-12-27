#include "pch.h"
#include "CTestLevel.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CPlayerScript.h"
#include "CLight3D.h"
#include "CCamera.h"
#include "CCameraMoveScript.h"
#include "CLevelMgr.h"

CTestLevel::CTestLevel()
{
}

CTestLevel::~CTestLevel()
{
}

void CTestLevel::begin()
{
    // Camera Object »ý¼º
    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);
    pCamObj->AddComponent(new CCameraMoveScript);

    pCamObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, -100.f));
    pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

    CLevelMgr::GetInst()->SetCameraObj(pCamObj);

    AddObject(pCamObj, 0);

    // Lights
    CGameObject* pLights = new CGameObject;
    pLights->SetName(L"Lights");

    pLights->AddComponent(new CTransform);
    pLights->AddComponent(new CMeshRender);

    pLights->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pLights->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

    pLights->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pLights->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pLights->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"YellowMaterial"));

    CGameObject* pLight1 = new CGameObject;
    pLight1->SetName(L"Light 1");

    pLight1->AddComponent(new CTransform);
    pLight1->AddComponent(new CMeshRender);
    pLight1->AddComponent(new CLight3D(LIGHT_TYPE::DIRECTIONAL, 0));
    pLight1->Light3D()->SetStrength(Vec3(1.f, 0.f, 0.f));

    pLight1->Transform()->SetRelativePos(Vec3(-750.f, 0.f, 100.f));
    pLight1->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

    pLight1->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pLight1->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pLight1->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"YellowMaterial"));

    pLights->AddChild(pLight1);

    CGameObject* pLight2 = new CGameObject;
    pLight2->SetName(L"Light 2");

    pLight2->AddComponent(new CTransform);
    pLight2->AddComponent(new CMeshRender);
    pLight2->AddComponent(new CLight3D(LIGHT_TYPE::DIRECTIONAL, 1));
    pLight2->Light3D()->SetStrength(Vec3(0.f, 1.f, 0.f));

    pLight2->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
    pLight2->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

    pLight2->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pLight2->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pLight2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"YellowMaterial"));

    pLights->AddChild(pLight2);

    CGameObject* pLight3 = new CGameObject;
    pLight3->SetName(L"Light 3");

    pLight3->AddComponent(new CTransform);
    pLight3->AddComponent(new CMeshRender);
    pLight3->AddComponent(new CLight3D(LIGHT_TYPE::DIRECTIONAL, 2));
    pLight3->Light3D()->SetStrength(Vec3(0.f, 0.f, 1.f));

    pLight3->Transform()->SetRelativePos(Vec3(750.f, 0.f, 100.f));
    pLight3->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

    pLight3->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pLight3->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pLight3->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"YellowMaterial"));

    pLights->AddChild(pLight3);

    AddObject(pLights, 1);

    AddMeshes();

    AddModels();

    CLevel::begin();
}

void CTestLevel::tick()
{
    CLevel::tick();
}

void CTestLevel::finaltick()
{
    CLevel::finaltick();
}

void CTestLevel::render()
{
    CLevel::render();
}

void CTestLevel::AddMeshes()
{
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
    pCircle->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    pMeshes->AddChild(pCircle);

    AddObject(pMeshes, 10);
}

void CTestLevel::AddModels()
{
    // Models
    CGameObject* pModels = new CGameObject;
    pModels->SetName(L"Models");

    pModels->AddComponent(new CTransform);
    pModels->AddComponent(new CMeshRender);
    pModels->AddComponent(new CPlayerScript);

    pModels->Transform()->SetRelativePos(Vec3(0.f, 500.f, 0.f));
    pModels->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

    pModels->MeshRender()->AddMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pModels->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pModels->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RedMaterial"));

    // Zelda Model
    CGameObject* pZelda = new CGameObject;
    pZelda->SetName(L"Zelda");

    pZelda->AddComponent(new CTransform);
    pZelda->AddComponent(new CMeshRender);
    pZelda->AddComponent(new CPlayerScript);

    pZelda->Transform()->SetRelativePos(Vec3(-500.f, 250.f, 0.f));
    pZelda->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pZelda->Transform()->SetAbsolute(true);

    pZelda->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pZelda->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    for (auto& mesh : CAssetMgr::GetInst()->FindModel(L"Zelda"))
    {
        pZelda->MeshRender()->AddMesh(mesh);
    }

    pModels->AddChild(pZelda);

    // damaged helmet
    CGameObject* pHelmet = new CGameObject;
    pHelmet->SetName(L"Damaged Helmet");

    pHelmet->AddComponent(new CTransform);
    pHelmet->AddComponent(new CMeshRender);
    pHelmet->AddComponent(new CPlayerScript);

    pHelmet->Transform()->SetRelativePos(Vec3(-250.f, 250.f, 0.f));
    pHelmet->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pHelmet->Transform()->SetAbsolute(true);

    pHelmet->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pHelmet->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    for (auto& mesh : CAssetMgr::GetInst()->FindModel(L"damaged_helmet"))
    {
        pHelmet->MeshRender()->AddMesh(mesh);
    }

    pModels->AddChild(pHelmet);

    // blue whale
    CGameObject* pWhale = new CGameObject;
    pWhale->SetName(L"blue whale");

    pWhale->AddComponent(new CTransform);
    pWhale->AddComponent(new CMeshRender);
    pWhale->AddComponent(new CPlayerScript);

    pWhale->Transform()->SetRelativePos(Vec3(0.f, 250.f, 0.f));
    pWhale->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pWhale->Transform()->SetAbsolute(true);

    pWhale->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pWhale->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    for (auto& mesh : CAssetMgr::GetInst()->FindModel(L"blue_whale"))
    {
        pWhale->MeshRender()->AddMesh(mesh);
    }

    pModels->AddChild(pWhale);

    // torii_gate
    CGameObject* pGate = new CGameObject;
    pGate->SetName(L"torii gate");

    pGate->AddComponent(new CTransform);
    pGate->AddComponent(new CMeshRender);
    pGate->AddComponent(new CPlayerScript);

    pGate->Transform()->SetRelativePos(Vec3(250.f, 250.f, 0.f));
    pGate->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pGate->Transform()->SetAbsolute(true);

    pGate->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pGate->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    for (auto& mesh : CAssetMgr::GetInst()->FindModel(L"torii_gate"))
    {
        pGate->MeshRender()->AddMesh(mesh);
    }

    pModels->AddChild(pGate);

    // dragon_warrior
    CGameObject* pDragon_warrior = new CGameObject;
    pDragon_warrior->SetName(L"dragon_warrior");

    pDragon_warrior->AddComponent(new CTransform);
    pDragon_warrior->AddComponent(new CMeshRender);
    pDragon_warrior->AddComponent(new CPlayerScript);

    pDragon_warrior->Transform()->SetRelativePos(Vec3(500.f, 250.f, 0.f));
    pDragon_warrior->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pDragon_warrior->Transform()->SetAbsolute(true);

    pDragon_warrior->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"BlinnPhong"));
    pDragon_warrior->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WhiteMaterial"));

    for (auto& mesh : CAssetMgr::GetInst()->FindModel(L"dragon_warrior"))
    {
        pDragon_warrior->MeshRender()->AddMesh(mesh);
    }

    pModels->AddChild(pDragon_warrior);

    AddObject(pModels, 10);
}
