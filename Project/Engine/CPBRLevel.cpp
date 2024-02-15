#include "pch.h"
#include "CPBRLevel.h"

#include "CGameObject.h"
#include "components.h"
#include "CCameraMoveScript.h"

#include "CScriptMgr.h"
#include "CPathMgr.h"

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

    AddSample();
    // AddModels();

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
    pSkyBox->AddComponent(new CSkyBox);

    pSkyBox->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pSkyBox->Transform()->SetRelativeScale(Vec3(5000.f, 5000.f, 5000.f));
    pSkyBox->Transform()->SetAbsolute(true);

    pSkyBox->SkyBox()->SetType(SKYBOX_TYPE::moonless);
    pSkyBox->SkyBox()->SetShape(SKYBOX_SHAPE::BOX);
 
    AddObject(pSkyBox, L"SkyBox");

    // BlinnPhong Obj
    CGameObject* pPhongObj = new CGameObject;
    pPhongObj->SetName(L"BlinnPhong Object");
    pPhongObj->AddComponent(new CTransform);
    pPhongObj->AddComponent(new CMeshRender);

    pPhongObj->Transform()->SetRelativePos(Vec3(-250.f, 0.f, 0.f));
    pPhongObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

    pPhongObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pPhongObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlinnPhongMtrl"));

    AddObject(pPhongObj, 0);

    // Mirror
    CGameObject* pMirrorObj = new CGameObject;
    pMirrorObj->SetName(L"Mirror");
    pMirrorObj->AddComponent(new CTransform);
    pMirrorObj->AddComponent(new CMeshRender);

    pMirrorObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 1000.f));
    pMirrorObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
    pMirrorObj->Transform()->SetRelativeScale(Vec3(2500.f, 500.f, 1.f));

    pMirrorObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    pMirrorObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"MirrorMtrl"));

    AddObject(pMirrorObj, 0);

    CGameObject* pFloor = new CGameObject;
    pFloor->SetName(L"Floor");
    pFloor->AddComponent(new CTransform);
    pFloor->AddComponent(new CMeshRender);

    pFloor->Transform()->SetRelativePos(Vec3(0.f, -250.f, 0.f));
    pFloor->Transform()->SetRelativeScale(Vec3(500.f, 100.f, 500.f));

    pFloor->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    pFloor->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRMtrl"));

    AddObject(pFloor, 0);

    CLevel::begin();
}

void CPBRLevel::tick()
{
    CLevel::tick();
}

void CPBRLevel::finaltick()
{
    CLevel::finaltick();
}

void CPBRLevel::AddSample()
{
    // ==========================
    // worn-painted-metal-ue
    // ==========================
    vector<tMeshData> meshes;
    meshes.push_back(CAssetMgr::GetInst()->MakeSphere(1, 50, 50));
    meshes[0].AmbientTextureFilename = "worn-painted-metal_albedo.png";
    meshes[0].AoTextureFilename = "worn-painted-metal_ao.png";
    meshes[0].NormalTextureFilename = "worn-painted-metal_normal-dx.png";
    meshes[0].HeightTextureFilename = "worn-painted-metal_height.png";
    meshes[0].MetallicTextureFilename = "worn-painted-metal_metallic.png";
    meshes[0].RoughnessTextureFilename = "worn-painted-metal_roughness.png";
    meshes[0].RelativeTextureFilePath = "Developers\\Textures\\PBR\\worn-painted-metal-ue\\";

    CGameObject* pObj = CAssetMgr::GetInst()->LoadModel(L"worn-painted", meshes);
    pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pObj->Transform()->SetAbsolute(true);

    AddObject(pObj, 0);

    // ==========================
    // brick-wall
    // ==========================
    meshes.clear();
    meshes.push_back(CAssetMgr::GetInst()->MakeSphere(1, 50, 50));
    meshes[0].AmbientTextureFilename = "brick-wall_albedo.png";
    meshes[0].AoTextureFilename = "brick-wall_ao.png";
    meshes[0].NormalTextureFilename = "brick-wall_normal-dx.png";
    meshes[0].HeightTextureFilename = "brick-wall_height.png";
    meshes[0].MetallicTextureFilename = "brick-wall_metallic.png";
    meshes[0].RoughnessTextureFilename = "brick-wall_roughness.png";
    meshes[0].RelativeTextureFilePath = "Developers\\Textures\\PBR\\brick-wall-ue\\";

    pObj = CAssetMgr::GetInst()->LoadModel(L"brick-wall", meshes);
    pObj->Transform()->SetRelativePos(Vec3(250.f, 0.f, 0.f));
    pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pObj->Transform()->SetAbsolute(true);

    AddObject(pObj, 0);

    // ==========================
    // columned-lava-rock-ue
    // ==========================
    meshes.clear();
    meshes.push_back(CAssetMgr::GetInst()->MakeSphere(1, 50, 50));
    meshes[0].AmbientTextureFilename = "columned-lava-rock_albedo.png";
    meshes[0].AoTextureFilename = "columned-lava-rock_ao.png";
    meshes[0].NormalTextureFilename = "columned-lava-rock_normal-dx.png";
    meshes[0].HeightTextureFilename = "columned-lava-rock_height.png";
    meshes[0].MetallicTextureFilename = "columned-lava-rock_metallic.png";
    meshes[0].RoughnessTextureFilename = "columned-lava-rock_roughness.png";
    meshes[0].EmissiveTextureFilename = "columned-lava-rock_emissive.png";
    meshes[0].RelativeTextureFilePath = "Developers\\Textures\\PBR\\columned-lava-rock-ue\\";

    pObj = CAssetMgr::GetInst()->LoadModel(L"columned-lava-rock", meshes);
    pObj->Transform()->SetRelativePos(Vec3(500.f, 0.f, 0.f));
    pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pObj->Transform()->SetAbsolute(true);

    AddObject(pObj, 0);

    // ==========================
    // wrinkled-paper-ue
    // ==========================
    meshes.clear();
    meshes.push_back(CAssetMgr::GetInst()->MakeSphere(1, 50, 50));
    meshes[0].AmbientTextureFilename = "wrinkled-paper-albedo.png";
    meshes[0].AoTextureFilename = "wrinkled-paper-ao.png";
    meshes[0].NormalTextureFilename = "wrinkled-paper-normal-dx.png";
    meshes[0].HeightTextureFilename = "wrinkled-paper-height.png";
    meshes[0].MetallicTextureFilename = "wrinkled-paper-metalness.png";
    meshes[0].RoughnessTextureFilename = "wrinkled-paper-roughness.png";
    meshes[0].RelativeTextureFilePath = "Developers\\Textures\\PBR\\wrinkled-paper-ue\\";

    pObj = CAssetMgr::GetInst()->LoadModel(L"wrinkled-paper", meshes);
    pObj->Transform()->SetRelativePos(Vec3(750.f, 0.f, 0.f));
    pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
    pObj->Transform()->SetAbsolute(true);

    AddObject(pObj, 0);
}

void CPBRLevel::AddModels()
{
    // Zelda Model
    CGameObject* pZelda =
        CAssetMgr::GetInst()->LoadModel(L"Zelda", "Developers\\Models\\zeldaPosed001\\", "zeldaPosed001.fbx");
    if (nullptr != pZelda)
    {
        pZelda->Transform()->SetRelativePos(Vec3(-500.f, 250.f, 0.f));
        pZelda->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

        AddObject(pZelda, L"Model");
    }

    // Damaged Helmet
    CGameObject* pDamagedHelmet = CAssetMgr::GetInst()->LoadModel(
        L"Damaged Helmet", "Developers\\Models\\damaged-helmet\\", "DamagedHelmet.gltf");
    if (nullptr != pDamagedHelmet)
    {
        pDamagedHelmet->Transform()->SetRelativePos(Vec3(-250.f, 250.f, 0.f));
        pDamagedHelmet->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

        AddObject(pDamagedHelmet, L"Model");
    }

    // torii gate
    CGameObject* ptoriigate =
        CAssetMgr::GetInst()->LoadModel(L"torii gate", "Developers\\Models\\torii_gate\\", "scene.gltf", true);
    if (nullptr != ptoriigate)
    {
        ptoriigate->Transform()->SetRelativePos(Vec3(0.f, 250.f, 0.f));
        ptoriigate->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

        AddObject(ptoriigate, L"Model");
    }

    // dragon warrior
    CGameObject* pDragonWarrior =
        CAssetMgr::GetInst()->LoadModel(L"Dragon Warrior", "Developers\\Models\\dragon_warrior\\", "scene.gltf", true);
    if (nullptr != pDragonWarrior)
    {
        pDragonWarrior->Transform()->SetRelativePos(Vec3(250.f, 250.f, 0.f));
        pDragonWarrior->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

        AddObject(pDragonWarrior, L"Model");
    }

    // ==========================
    // PBR .fbx 포맷
    // ==========================
    tMeshData meshData = {};
    meshData.AmbientTextureFilename = "angel_armor_albedo.jpg";
    meshData.NormalTextureFilename = "angel_armor_normal.jpg";
    meshData.MetallicTextureFilename = "angel_armor_metalness.jpg";
    meshData.RoughnessTextureFilename = "angel_armor_roughness.jpg";
    meshData.EmissiveTextureFilename = "angel_armor_e.jpg";
    meshData.RelativeTextureFilePath = "Developers\\Models\\armored-female-future-soldier\\";

    CGameObject* pArmoredFemale = CAssetMgr::GetInst()->LoadModel(
        L"angel_armor", "Developers\\Models\\armored-female-future-soldier\\", "angel_armor.fbx", false, meshData);
    if (nullptr != pArmoredFemale)
    {
        pArmoredFemale->Transform()->SetRelativePos(Vec3(500.f, 250.f, 0.f));
        pArmoredFemale->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
        AddObject(pArmoredFemale, L"Model");
    }
}
