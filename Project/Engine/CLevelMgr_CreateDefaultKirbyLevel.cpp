#include "pch.h"
#include "CLevelMgr.h"
#include <Scripts\CScriptMgr.h>
#include "CScript.h"
#include "CEngine.h"
#include "CLevel.h"
#include "CEditorMgr.h"
#include "CEditor.h"

#include "CLevelSaveLoad.h"
#include "components.h"

CLevel* CLevelMgr::CreateDefaultKirbyLevel()
{
    CLevel* NewLevel = new CLevel;
    NewLevel->SetName(L"Default Kirby Level");

    // =============
    // create layer
    // =============
    NewLevel->GetLayer(0)->SetName(L"Default");
    NewLevel->GetLayer(1)->SetName(L"Manager");
    NewLevel->GetLayer(2)->SetName(L"World Static");
    NewLevel->GetLayer(3)->SetName(L"World Dynamic");
    NewLevel->GetLayer(4)->SetName(L"Player");
    NewLevel->GetLayer(5)->SetName(L"Monster");
    NewLevel->GetLayer(6)->SetName(L"Player Attack");
    NewLevel->GetLayer(7)->SetName(L"Monster Attack");
    NewLevel->GetLayer(8)->SetName(L"Layer 8");
    NewLevel->GetLayer(9)->SetName(L"Layer 9");
    NewLevel->GetLayer(10)->SetName(L"Layer 10");
    NewLevel->GetLayer(11)->SetName(L"Layer 11");
    NewLevel->GetLayer(12)->SetName(L"Layer 12");
    NewLevel->GetLayer(13)->SetName(L"Layer 13");
    NewLevel->GetLayer(14)->SetName(L"Effect");
    NewLevel->GetLayer(15)->SetName(L"UI");

    // layer collision check
    CPhysicsMgr::GetInst()->LayerCheck(4, 4, false);

    for (int i = 0; i < LAYER_MAX; ++i)
    {
        CPhysicsMgr::GetInst()->LayerCheck(0, i, false);
        CPhysicsMgr::GetInst()->LayerCheck(1, i, false);
        CPhysicsMgr::GetInst()->LayerCheck(14, i, false);
        CPhysicsMgr::GetInst()->LayerCheck(15, i, false);
    }

    // =============
    // world setting
    // =============
    CPhysicsMgr::GetInst()->SetPPM(100.f);

    // Manager
    CGameObject* pManager = new CGameObject;
    pManager->SetName(L"Manager");
    pManager->AddComponent(new CTransform);
    pManager->AddComponent(CScriptMgr::GetScript(PLAYERMGR));

    NewLevel->AddObject(pManager, 1);

    // =============
    // create camera
    // =============
    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CMeshRender);
    pCamObj->AddComponent(new CCamera);

    pCamObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
    pCamObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CameraIconMtrl"), 0);
    pCamObj->MeshRender()->SetFrustumCheck(false);
    pCamObj->MeshRender()->SetCastShadow(false);

    pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerMaskAll();
    pCamObj->Camera()->LayerMask(NewLevel, L"UI", false);
    pCamObj->Camera()->SetHDRI(true);
    pCamObj->Camera()->SetFOV(XMConvertToRadians(60.f));

    NewLevel->AddObject(pCamObj, 0);

    // UI Camera
    CGameObject* pUICamObj = new CGameObject;
    pUICamObj->SetName(L"UI Camera");
    pUICamObj->AddComponent(new CTransform);
    pUICamObj->AddComponent(new CMeshRender);
    pUICamObj->AddComponent(new CCamera);

    pUICamObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
    pUICamObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CameraIconMtrl"), 0);
    pUICamObj->MeshRender()->SetFrustumCheck(false);
    pUICamObj->MeshRender()->SetCastShadow(false);

    pUICamObj->Camera()->SetCameraPriority(1);
    pUICamObj->Camera()->LayerMask(NewLevel, L"UI", true);

    NewLevel->AddObject(pUICamObj, 0);

    // =============
    // create Env
    // =============
    //  SkyBox
    CGameObject* pSkyBoxObj = CAssetMgr::GetInst()->LoadFBX(L"fbx\\LevelObject\\Skybox\\Day\\Day.fbx")->Instantiate();
    pSkyBoxObj->SetName(L"SkyBox");

    pSkyBoxObj->Transform()->SetLocalRotation(Vec3(-XM_PIDIV2, XM_PI, 0.f));
    pSkyBoxObj->Transform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));

    pSkyBoxObj->GetRenderComponent()->SetFrustumCheck(false);
    pSkyBoxObj->GetRenderComponent()->SetCastShadow(false);

    Ptr<CMaterial> SkyBoxMtrl = pSkyBoxObj->GetRenderComponent()->GetMaterial(0);
    SkyBoxMtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"KirbySkySphereShader"));
    SkyBoxMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"fbx\\LevelObject\\Skybox\\Day\\SkySphere_DayTest.1151417570.png"));
    SkyBoxMtrl->Save(SkyBoxMtrl->GetKey());

    NewLevel->AddObject(pSkyBoxObj, 0);

    // Light
    CGameObject* pDirLight = new CGameObject;
    pDirLight->SetName(L"Directional Light");
    pDirLight->AddComponent(new CTransform);
    pDirLight->AddComponent(new CLight);

    pDirLight->Transform()->SetLocalPos(Vec3(0.f, 2000.f, -2000.f));
    pDirLight->Transform()->SetLocalRotation(Vec3(XMConvertToRadians(45.f), 0.f, 0.f));
    pDirLight->Light()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
    pDirLight->Light()->SetRadius(10.f);

    NewLevel->AddObject(pDirLight, 0);

    // Floor
    CGameObject* pFloor = new CGameObject;
    pFloor->SetName(L"Floor");
    pFloor->AddComponent(new CTransform);
    pFloor->AddComponent(new CMeshRender);
    pFloor->AddComponent(new CBoxCollider);

    pFloor->Transform()->SetLocalScale(Vec3(10000.f, 1.f, 10000.f));
    pFloor->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"BoxMesh"));
    pFloor->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredMtrl"), 0);
    pFloor->MeshRender()->GetMaterial(0)->SetAlbedo(Vec4(1.f, 1.f, 1.f, 1.f));
    pFloor->MeshRender()->SetFrustumCheck(false);

    NewLevel->AddObject(pFloor, 2);

    // ==================
    // create main Player
    // ==================
    // Main Player
    CGameObject* pPlayer = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Characters\\Kirby\\Base\\Kirby.fbx")->Instantiate();
    pPlayer->SetName(L"Main Player");
    pPlayer->AddComponent(new CCharacterController);
    pPlayer->AddComponent(CScriptMgr::GetScript(KIRBYMOVECONTROLLER));
    pPlayer->AddComponent(CScriptMgr::GetScript(KIRBYUNITSCRIPT));
    pPlayer->AddComponent(CScriptMgr::GetScript(KIRBYFSM));

    pPlayer->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
    pPlayer->Transform()->SetLocalPos(Vec3(0.f, 10.f, 0.f));
    pPlayer->MeshRender()->SetFrustumCheck(false);
    pPlayer->CharacterController()->SetCenter(Vec3(0.f, 50.f, 0.f));
    pPlayer->CharacterController()->SetHeight(1.6f);
    pPlayer->CharacterController()->SetMinMoveDistance(0.f);

    Ptr<CMaterial> pPlayerMtrl = pPlayer->GetRenderComponent()->GetMaterial(0);
    Ptr<CGraphicsShader> pPlayerShader = CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"KirbyBodyShader");

    for (UINT i = 0; i < pPlayer->GetRenderComponent()->GetMtrlCount(); ++i)
    {
        wstring MtrlName = pPlayer->GetRenderComponent()->GetMesh()->GetIBName(i);

        if (MtrlName != L"BodyM__BodyC" && MtrlName != L"limbsM__BodyC" && MtrlName != L"MouthNormalM__BodyC")
        {
            pPlayer->GetRenderComponent()->SetMaterial(nullptr, i);
        }
    }

    if (pPlayerMtrl != nullptr && pPlayerShader != nullptr)
    {
        pPlayerMtrl->SetShader(pPlayerShader);
        pPlayerMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"fbx\\Characters\\Kirby\\Base\\KirbyEye.00.png"));
        pPlayerMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"fbx\\Characters\\Kirby\\Base\\KirbyEyeMask.00.png"));
        pPlayerMtrl->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"fbx\\Characters\\Kirby\\Base\\KirbyEyeNormal.00.png"));
        pPlayerMtrl->SetTexParam(TEX_PARAM::TEX_3,
                                 CAssetMgr::GetInst()->FindAsset<CTexture>(L"fbx\\Characters\\Kirby\\Base\\KirbySkin.856436594.png"));
        pPlayerMtrl->SetTexParam(TEX_PARAM::TEX_4,
                                 CAssetMgr::GetInst()->FindAsset<CTexture>(L"fbx\\Characters\\Kirby\\Base\\KirbyMouth.1209505089.png"));
    }

    NewLevel->AddObject(pPlayer, 4);

    // Player Vacuum Collider
    CGameObject* pVacuumCol = new CGameObject;
    pVacuumCol->SetName(L"Vacuum Collider");
    pVacuumCol->AddComponent(new CTransform);
    pVacuumCol->AddComponent(new CSphereCollider);
    pVacuumCol->AddComponent(CScriptMgr::GetScript(KIRBYVACUUMCOLLIDER));

    pVacuumCol->Transform()->SetAbsolute(false);
    pVacuumCol->Transform()->SetLocalPos(Vec3(0.f, 1.7f, 1.7f));
    pVacuumCol->Transform()->SetLocalScale(Vec3(1.5f, 1.5f, 1.5f));
    pVacuumCol->SphereCollider()->SetTrigger(true);

    pPlayer->AddChild(pVacuumCol);

    // Player Body Collider
    CGameObject* pBodyCollider = new CGameObject;
    pBodyCollider->SetName(L"Body Collider");
    pBodyCollider->AddComponent(new CTransform);
    pBodyCollider->AddComponent(new CCapsuleCollider);
    pBodyCollider->AddComponent(CScriptMgr::GetScript(KIRBYBODYCOLLIDER));

    pBodyCollider->Transform()->SetAbsolute(false);
    pBodyCollider->Transform()->SetLocalScale(Vec3(1.1f, 1.1f, 1.1f));
    pBodyCollider->CapsuleCollider()->SetTrigger(true);
    pBodyCollider->CapsuleCollider()->SetCenter(Vec3(0.f, 50.f, 0.f));
    pBodyCollider->CapsuleCollider()->SetHeight(1.6f);

    pPlayer->AddChild(pBodyCollider);

    // DeformLight PointLight
    CGameObject* pPointLight = new CGameObject;
    pPointLight->SetName(L"DeformLight PointLight");
    pPointLight->AddComponent(new CTransform);
    pPointLight->AddComponent(new CLight);

    pPointLight->Transform()->SetLocalPos(Vec3(0.f, 150.f, 0.f));
    pPointLight->Transform()->SetLocalRotation(Vec3(0.f, XMConvertToRadians(-60.f), 0.f));
    pPointLight->Light()->SetLightType(LIGHT_TYPE::POINT);
    pPointLight->Light()->SetLightRadiance(Vec3(255.f, 226.f, 217.f) / 255.f);
    pPointLight->Light()->SetRadius(80.f);
    pPointLight->Light()->SetFallOffEnd(750.f);
    pPointLight->Light()->SetHaloRadius(160.f);
    pPointLight->Light()->SetHaloStrength(0.125f);

    pPlayer->AddChild(pPointLight);
    pPointLight->SetActive(false);

    // ==================
    // create default object
    // ==================
    // Box
    CGameObject* pBox = CAssetMgr::GetInst()->LoadFBX(L"fbx\\LevelObject\\Gimmick\\Block\\StarBlock\\H2W2\\StarBlock_H2W2.fbx")->Instantiate();
    pBox->SetName(L"Star Block");
    pBox->AddComponent(new CRigidbody);
    pBox->AddComponent(new CBoxCollider);

    pBox->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
    pBox->Transform()->SetLocalPos(Vec3(-500.f, 150.f, 500.f));
    pBox->BoxCollider()->SetCenter(Vec3(0.f, 100.f, 0.f)); 

    pBox->BoxCollider()->SetCenter(Vec3(0.f, 100.f, 0.f));

    pBox->GetRenderComponent()->SetBoundingRadius(300.f);

    NewLevel->AddObject(pBox, 3);

    // Monster
    CGameObject* pMonsterAbility = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Characters\\Monster\\NormalEnemy\\NormalEnemy.fbx")->Instantiate();
    pMonsterAbility->SetName(L"Monster (Ability)");
    pMonsterAbility->AddComponent(new CRigidbody);
    pMonsterAbility->AddComponent(new CCapsuleCollider);
    pMonsterAbility->AddComponent(CScriptMgr::GetScript(KIRBYCOPYABILITYSCRIPT));

    pMonsterAbility->Transform()->SetLocalPos(Vec3(280.f, 100.f, 280.f));
    pMonsterAbility->Transform()->SetLocalRotation(Vec3(0.f, XMConvertToRadians(180.f), 0.f));
    pMonsterAbility->Transform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));

    NewLevel->AddObject(pMonsterAbility, 5);

    return NewLevel;
}