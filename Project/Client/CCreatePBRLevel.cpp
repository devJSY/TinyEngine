#include "pch.h"
#include "CCreatePBRLevel.h"

#include <Engine/func.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CLevelMgr.h>

CLevel* CCreatePBRLevel::CreatePBRLevel()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    pCurLevel->SetName(L"PBR Level");

    for (int i = 0; i < LAYER_MAX; i++)
    {
        std::wstring Name = L"Layer ";
        Name += std::to_wstring(i);
        pCurLevel->GetLayer(i)->SetName(Name);
    }

    pCurLevel->GetLayer(0)->SetName(L"Default");
    pCurLevel->GetLayer(1)->SetName(L"Camera");
    pCurLevel->GetLayer(2)->SetName(L"Light");
    pCurLevel->GetLayer(3)->SetName(L"Player");
    pCurLevel->GetLayer(4)->SetName(L"Monster");
    pCurLevel->GetLayer(5)->SetName(L"Bullet");
    pCurLevel->GetLayer(6)->SetName(L"Effect");
    pCurLevel->GetLayer(7)->SetName(L"Tile");
    pCurLevel->GetLayer(8)->SetName(L"Model");
    pCurLevel->GetLayer(12)->SetName(L"BackGround");
    pCurLevel->GetLayer(13)->SetName(L"SkyBox");
    pCurLevel->GetLayer(14)->SetName(L"PostProcess");
    pCurLevel->GetLayer(15)->SetName(L"UI");

    // AddSample(pCurLevel);
    AddModels(pCurLevel);

    // Main Camera
    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerMaskAll();
    pCamObj->Camera()->LayerMask(pCurLevel, L"UI", false);
    pCamObj->Camera()->SetHDRI(true);

    pCurLevel->AddObject(pCamObj, L"Camera");

    // UI Camera
    pCamObj = new CGameObject;
    pCamObj->SetName(L"UI Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(1);
    pCamObj->Camera()->LayerMask(pCurLevel, L"UI", true);

    pCurLevel->AddObject(pCamObj, L"Camera");

    // SkyBox
    CGameObject* pSkyBox = new CGameObject;
    pSkyBox->SetName(L"SkyBox");

    pSkyBox->AddComponent(new CTransform);
    pSkyBox->AddComponent(new CSkyBox);

    pSkyBox->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
    pSkyBox->Transform()->SetLocalScale(Vec3(5000.f, 5000.f, 5000.f));
    pSkyBox->Transform()->SetAbsolute(true);

    pSkyBox->SkyBox()->SetShape(SKYBOX_SHAPE::BOX);

    pCurLevel->AddObject(pSkyBox, L"SkyBox");

    // BlinnPhong Obj
    CGameObject* pPhongObj = new CGameObject;
    pPhongObj->SetName(L"BlinnPhong Object");
    pPhongObj->AddComponent(new CTransform);
    pPhongObj->AddComponent(new CMeshRender);

    pPhongObj->Transform()->SetLocalPos(Vec3(-250.f, 0.f, 0.f));
    pPhongObj->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));

    pPhongObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
    pPhongObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlinnPhongMtrl"), 0);

    pCurLevel->AddObject(pPhongObj, 0);

    CGameObject* pFloor = new CGameObject;
    pFloor->SetName(L"Floor");
    pFloor->AddComponent(new CTransform);
    pFloor->AddComponent(new CMeshRender);

    pFloor->Transform()->SetLocalPos(Vec3(0.f, -250.f, 0.f));
    pFloor->Transform()->SetLocalScale(Vec3(2500.f, 100.f, 2500.f));

    pFloor->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    pFloor->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRMtrl"), 0);

    pCurLevel->AddObject(pFloor, 0);

    return pCurLevel;
}

void CCreatePBRLevel::AddSample(CLevel* _CurLevel)
{
    //// ==========================
    //// worn-painted-metal-ue
    //// ==========================
    // vector<tMeshData> meshes;
    // meshes.push_back(CAssetMgr::GetInst()->MakeSphere(1, 50, 50));
    // meshes[0].AmbientTextureFilename = "worn-painted-metal_albedo.png";
    // meshes[0].AoTextureFilename = "worn-painted-metal_ao.png";
    // meshes[0].NormalTextureFilename = "worn-painted-metal_normal-dx.png";
    // meshes[0].HeightTextureFilename = "worn-painted-metal_height.png";
    // meshes[0].MetallicTextureFilename = "worn-painted-metal_metallic.png";
    // meshes[0].RoughnessTextureFilename = "worn-painted-metal_roughness.png";
    // meshes[0].RelativeTextureFilePath = "Developers/Textures/PBR/worn-painted-metal-ue/";

    // CGameObject* pObj = CAssetMgr::GetInst()->LoadModel(L"worn-painted", meshes);
    // pObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
    // for (const auto& child : pObj->GetChildObject())
    //{
    //     child->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
    //     child->Transform()->SetAbsolute(true);
    // }
    //_CurLevel->AddObject(pObj, 0);

    //// ==========================
    //// brick-wall
    //// ==========================
    // meshes.clear();
    // meshes.push_back(CAssetMgr::GetInst()->MakeSphere(1, 50, 50));
    // meshes[0].AmbientTextureFilename = "brick-wall_albedo.png";
    // meshes[0].AoTextureFilename = "brick-wall_ao.png";
    // meshes[0].NormalTextureFilename = "brick-wall_normal-dx.png";
    // meshes[0].HeightTextureFilename = "brick-wall_height.png";
    // meshes[0].MetallicTextureFilename = "brick-wall_metallic.png";
    // meshes[0].RoughnessTextureFilename = "brick-wall_roughness.png";
    // meshes[0].RelativeTextureFilePath = "Developers/Textures/PBR/brick-wall-ue/";

    // pObj = CAssetMgr::GetInst()->LoadModel(L"brick-wall", meshes);
    // pObj->Transform()->SetLocalPos(Vec3(250.f, 0.f, 0.f));
    // for (const auto& child : pObj->GetChildObject())
    //{
    //     child->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
    //     child->Transform()->SetAbsolute(true);
    // }
    //_CurLevel->AddObject(pObj, 0);

    //// ==========================
    //// columned-lava-rock-ue
    //// ==========================
    // meshes.clear();
    // meshes.push_back(CAssetMgr::GetInst()->MakeSphere(1, 50, 50));
    // meshes[0].AmbientTextureFilename = "columned-lava-rock_albedo.png";
    // meshes[0].AoTextureFilename = "columned-lava-rock_ao.png";
    // meshes[0].NormalTextureFilename = "columned-lava-rock_normal-dx.png";
    // meshes[0].HeightTextureFilename = "columned-lava-rock_height.png";
    // meshes[0].MetallicTextureFilename = "columned-lava-rock_metallic.png";
    // meshes[0].RoughnessTextureFilename = "columned-lava-rock_roughness.png";
    // meshes[0].EmissiveTextureFilename = "columned-lava-rock_emissive.png";
    // meshes[0].RelativeTextureFilePath = "Developers/Textures/PBR/columned-lava-rock-ue/";

    // pObj = CAssetMgr::GetInst()->LoadModel(L"columned-lava-rock", meshes);
    // pObj->Transform()->SetLocalPos(Vec3(500.f, 0.f, 0.f));
    // for (const auto& child : pObj->GetChildObject())
    //{
    //     child->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
    //     child->Transform()->SetAbsolute(true);
    // }
    //_CurLevel->AddObject(pObj, 0);

    //// ==========================
    //// wrinkled-paper-ue
    //// ==========================
    // meshes.clear();
    // meshes.push_back(CAssetMgr::GetInst()->MakeSphere(1, 50, 50));
    // meshes[0].AmbientTextureFilename = "wrinkled-paper-albedo.png";
    // meshes[0].AoTextureFilename = "wrinkled-paper-ao.png";
    // meshes[0].NormalTextureFilename = "wrinkled-paper-normal-dx.png";
    // meshes[0].HeightTextureFilename = "wrinkled-paper-height.png";
    // meshes[0].MetallicTextureFilename = "wrinkled-paper-metalness.png";
    // meshes[0].RoughnessTextureFilename = "wrinkled-paper-roughness.png";
    // meshes[0].RelativeTextureFilePath = "Developers/Textures/PBR/wrinkled-paper-ue/";

    // pObj = CAssetMgr::GetInst()->LoadModel(L"wrinkled-paper", meshes);
    // pObj->Transform()->SetLocalPos(Vec3(750.f, 0.f, 0.f));
    // for (const auto& child : pObj->GetChildObject())
    //{
    //     child->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
    //     child->Transform()->SetAbsolute(true);
    // }
    //_CurLevel->AddObject(pObj, 0);
}

void CCreatePBRLevel::AddModels(CLevel* _CurLevel)
{
    //// Zelda Model
    // CGameObject* pZelda = CAssetMgr::GetInst()->LoadModel(L"Zelda", "Developers/Models/zeldaPosed001/", "zeldaPosed001.fbx");
    // if (nullptr != pZelda)
    //{
    //     pZelda->Transform()->SetLocalPos(Vec3(-500.f, 0.f, -250.f));
    //     pZelda->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));

    //    _CurLevel->AddObject(pZelda, L"Model");
    //}

    //// Damaged Helmet
    // CGameObject* pDamagedHelmet = CAssetMgr::GetInst()->LoadModel(L"Damaged Helmet", "Developers/Models/damaged-helmet/", "DamagedHelmet.gltf");
    // if (nullptr != pDamagedHelmet)
    //{
    //     pDamagedHelmet->Transform()->SetLocalPos(Vec3(-250.f, 0.f, -250.f));
    //     pDamagedHelmet->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));

    //    _CurLevel->AddObject(pDamagedHelmet, L"Model");
    //}

    //// torii gate
    // CGameObject* ptoriigate = CAssetMgr::GetInst()->LoadModel(L"torii gate", "Developers/Models/torii_gate/", "scene.gltf", true);
    // if (nullptr != ptoriigate)
    //{
    //     ptoriigate->Transform()->SetLocalPos(Vec3(0.f, 0.f, -250.f));
    //     ptoriigate->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));

    //    _CurLevel->AddObject(ptoriigate, L"Model");
    //}

    //// dragon warrior
    // CGameObject* pDragonWarrior = CAssetMgr::GetInst()->LoadModel(L"Dragon Warrior", "Developers/Models/dragon_warrior/", "scene.gltf", true);
    // if (nullptr != pDragonWarrior)
    //{
    //     pDragonWarrior->Transform()->SetLocalPos(Vec3(250.f, 0.f, -250.f));
    //     pDragonWarrior->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));

    //    _CurLevel->AddObject(pDragonWarrior, L"Model");
    //}

    //// ==========================
    //// PBR .fbx Æ÷¸Ë
    //// ==========================
    // tMeshData meshData = {};
    // meshData.AmbientTextureFilename = "angel_armor_albedo.jpg";
    // meshData.NormalTextureFilename = "angel_armor_normal.jpg";
    // meshData.MetallicTextureFilename = "angel_armor_metalness.jpg";
    // meshData.RoughnessTextureFilename = "angel_armor_roughness.jpg";
    // meshData.EmissiveTextureFilename = "angel_armor_e.jpg";
    // meshData.RelativeTextureFilePath = "Developers/Models/armored-female-future-soldier/";

    // CGameObject* pArmoredFemale =
    //     CAssetMgr::GetInst()->LoadModel(L"angel_armor", "Developers/Models/armored-female-future-soldier/", "angel_armor.fbx", false, meshData);
    // if (nullptr != pArmoredFemale)
    //{
    //     pArmoredFemale->Transform()->SetLocalPos(Vec3(500.f, 0.f, -250.f));
    //     pArmoredFemale->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
    //     _CurLevel->AddObject(pArmoredFemale, L"Model");
    // }
}
