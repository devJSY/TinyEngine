#include "pch.h"
#include "CPBRLevel.h"

#include "CGameObject.h"
#include "components.h"
#include "CScriptMgr.h"
#include "CCameraMoveScript.h"

CPBRLevel::CPBRLevel()
{
}

CPBRLevel::~CPBRLevel()
{
}

void CPBRLevel::begin()
{
    for (int i = 0; i < LAYER_MAX; i++)
    {
        wstring Name = L"Layer ";
        Name += std::to_wstring(i);
        GetLayer(i)->SetName(Name);
    }

    GetLayer(0)->SetName(L"Default");
    GetLayer(1)->SetName(L"Camera");
    GetLayer(2)->SetName(L"Light");
    GetLayer(3)->SetName(L"Player");
    GetLayer(4)->SetName(L"Monster");
    GetLayer(5)->SetName(L"Bullet");
    GetLayer(6)->SetName(L"Effect");
    GetLayer(7)->SetName(L"Tile");
    GetLayer(8)->SetName(L"Model");
    GetLayer(12)->SetName(L"BackGround");
    GetLayer(13)->SetName(L"SkyBox");
    GetLayer(14)->SetName(L"PostProcess");
    GetLayer(15)->SetName(L"UI");

    AddModels();

    // Main Camera
    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);
    pCamObj->AddComponent(CScriptMgr::GetScript(SCRIPT_TYPE::CAMERAMOVESCRIPT));

    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerCheckAll();
    pCamObj->Camera()->LayerCheck(L"UI", false);

    AddObject(pCamObj, L"Camera");

    // UI 만 렌더링
    pCamObj = new CGameObject;
    pCamObj->SetName(L"UI Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetUICamera();

    AddObject(pCamObj, L"Camera");

    // SkyBox
    CGameObject* pSkyBox = new CGameObject;
    pSkyBox->SetName(L"SkyBox");

    pSkyBox->AddComponent(new CTransform);
    pSkyBox->AddComponent(new CMeshRender);

    pSkyBox->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pSkyBox->Transform()->SetRelativeScale(Vec3(5000.f, 5000.f, 5000.f));
    pSkyBox->Transform()->SetAbsolute(true);

    pSkyBox->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pSkyBox->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PureSkyBoxMtrl"));

    AddObject(pSkyBox, L"SkyBox");

    // Test Obj
    CGameObject* pObj = new CGameObject;
    pObj->SetName(L"Sphere");

    pObj->AddComponent(new CTransform);
    pObj->AddComponent(new CMeshRender);

    pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pObj->Transform()->SetAbsolute(true);

    pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRMtrl"));

    AddObject(pObj, 0);
}

void CPBRLevel::tick()
{
    CLevel::tick();
}

void CPBRLevel::finaltick()
{
    CLevel::finaltick();
}

void CPBRLevel::AddModels()
{
    // Zelda Model
    auto meshes = CAssetMgr::GetInst()->ReadFromFile("Developers\\Models\\zeldaPosed001\\", "zeldaPosed001.fbx");
    CGameObject* pZelda = CAssetMgr::GetInst()->LoadModel(meshes, L"Zelda");
    if (nullptr != pZelda)
    {
        pZelda->Transform()->SetRelativePos(Vec3(-500.f, 250.f, 0.f));
        pZelda->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

        AddObject(pZelda, L"Model");
    }

    // Damaged Helmet
    meshes = CAssetMgr::GetInst()->ReadFromFile("Developers\\Models\\damaged-helmet\\", "DamagedHelmet.gltf");
    CGameObject* pDamagedHelmet = CAssetMgr::GetInst()->LoadModel(meshes, L"Damaged Helmet");
    if (nullptr != pDamagedHelmet)
    {
        pDamagedHelmet->Transform()->SetRelativePos(Vec3(-250.f, 250.f, 0.f));
        pDamagedHelmet->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

        AddObject(pDamagedHelmet, L"Model");
    }

    // blue whale
    meshes = CAssetMgr::GetInst()->ReadFromFile("Developers\\Models\\blue_whale\\", "scene.gltf");
    CGameObject* pblueWhale = CAssetMgr::GetInst()->LoadModel(meshes, L"blue whale");
    if (nullptr != pblueWhale)
    {
        pblueWhale->Transform()->SetRelativePos(Vec3(0.f, 250.f, 0.f));
        pblueWhale->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

        AddObject(pblueWhale, L"Model");
    }

    // torii gate
    meshes = CAssetMgr::GetInst()->ReadFromFile("Developers\\Models\\torii_gate\\", "scene.gltf", true);
    CGameObject* ptoriigate = CAssetMgr::GetInst()->LoadModel(meshes, L"torii gate");
    if (nullptr != ptoriigate)
    {
        ptoriigate->Transform()->SetRelativePos(Vec3(250.f, 250.f, 0.f));
        ptoriigate->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

        AddObject(ptoriigate, L"Model");
    }

    // dragon warrior
    meshes = CAssetMgr::GetInst()->ReadFromFile("Developers\\Models\\dragon_warrior\\", "scene.gltf", true);
    CGameObject* pDragonWarrior = CAssetMgr::GetInst()->LoadModel(meshes, L"Dragon Warrior");
    if (nullptr != pDragonWarrior)
    {
        pDragonWarrior->Transform()->SetRelativePos(Vec3(500.f, 250.f, 0.f));
        pDragonWarrior->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

        AddObject(pDragonWarrior, L"Model");
    }

    // .fbx 포맷
    std::string path = "Developers\\Models\\armored-female-future-soldier\\";
    meshes = CAssetMgr::GetInst()->ReadFromFile(path, "angel_armor.fbx", false);
    meshes[0].AmbientTextureFilename = "angel_armor_albedo.jpg";
    meshes[0].NormalTextureFilename = "angel_armor_normal.jpg";
    meshes[0].MetallicTextureFilename = "angel_armor_metalness.jpg";
    meshes[0].RoughnessTextureFilename = "angel_armor_roughness.jpg";
    meshes[0].EmissiveTextureFilename = "angel_armor_e.jpg";

    CGameObject* pArmoredFemale = CAssetMgr::GetInst()->LoadModel(meshes, L"angel_armor");
    if (nullptr != pArmoredFemale)
    {
        pArmoredFemale->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
        AddObject(pArmoredFemale, L"Model");
    }
}
