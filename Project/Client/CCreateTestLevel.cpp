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

    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerMaskAll();
    pCamObj->Camera()->LayerMask(pCurLevel, L"UI", false);
    pCamObj->Camera()->SetHDRI(false);

    pCurLevel->AddObject(pCamObj, 0);

    // UI ¸¸ ·»´õ¸µ
    pCamObj = new CGameObject;
    pCamObj->SetName(L"UI Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(1);
    pCamObj->Camera()->LayerMask(pCurLevel, L"UI", true);

    pCurLevel->AddObject(pCamObj, 0);

    // ±¤¿ø Ãß°¡
    CGameObject* pLight = new CGameObject;
    pLight->SetName(L"Light 1");
    pLight->AddComponent(new CTransform);
    pLight->AddComponent(new CMeshRender);
    pLight->AddComponent(new CLight2D);

    pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

    pCurLevel->AddObject(pLight, 0);

    GamePlayStatic::ChangeLevelState(pCurLevel, LEVEL_STATE::STOP);
}
