#include "pch.h"
#include "CCreateTestLevel.h"

#include <Engine\\func.h>
#include <Engine\\CLevel.h>
#include <Engine\\CLayer.h>
#include <Engine\\CGameObject.h>
#include <Engine\\components.h>
#include <Engine\\CScript.h>

#include <Engine\\CLevelMgr.h>
#include <Scripts\\CScriptMgr.h>

void CCreateTestLevel::Init()
{
    //// °øÆ÷Åº
    // CGameObject* pShockWave = new CGameObject;
    // pShockWave->SetName(L"Shock Wave");
    // pShockWave->AddComponent(new CTransform);
    // pShockWave->AddComponent(new CMeshRender);
    // pShockWave->AddComponent(CScriptMgr::GetScript(SHOCKWAVESCRIPT));

    // pShockWave->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1.f));
    // pShockWave->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    // pShockWave->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShockWaveMtrl"));

    // Ptr<CPrefab> pMissilePrefab = new CPrefab(pShockWave);
    // CAssetMgr::GetInst()->AddAsset<CPrefab>(L"ShockWavePrefab", pMissilePrefab.Get());

    // pMissilePrefab->Save(L"prefab\\ShockWavePrefab.pref");
}

void CCreateTestLevel::CreateTestLevel()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    pCurLevel->SetName(L"Test Level");

    for (int i = 0; i < LAYER_MAX; i++)
    {
        wstring Name = L"Layer ";
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
    pCurLevel->GetLayer(13)->SetName(L"BackGround");
    pCurLevel->GetLayer(14)->SetName(L"PostProcess");
    pCurLevel->GetLayer(15)->SetName(L"UI");

    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerMaskAll();
    pCamObj->Camera()->LayerMask(pCurLevel, L"UI", false);
    pCamObj->Camera()->SetHDRI(false);

    pCurLevel->AddObject(pCamObj, L"Camera");

    // UI ¸¸ ·»´õ¸µ
    pCamObj = new CGameObject;
    pCamObj->SetName(L"UI Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(1);
    pCamObj->Camera()->LayerMask(pCurLevel, L"UI", true);

    pCurLevel->AddObject(pCamObj, L"Camera");

    // Player
    CGameObject* pPlayer = new CGameObject;
    pPlayer->SetName(L"Player");

    pPlayer->AddComponent(new CTransform);
    pPlayer->AddComponent(new CMeshRender);

    pPlayer->Transform()->SetAbsolute(true);
    pPlayer->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pPlayer->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

    pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

    pCurLevel->AddObject(pPlayer, L"Player");

    // ±¤¿ø Ãß°¡
    CGameObject* pLight = new CGameObject;
    pLight->SetName(L"Light 1");
    pLight->AddComponent(new CTransform);
    pLight->AddComponent(new CMeshRender);
    pLight->AddComponent(new CLight2D);

    pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

    pCurLevel->AddObject(pLight, L"Light");

    GamePlayStatic::ChangeLevelState(pCurLevel, LEVEL_STATE::STOP);
}
