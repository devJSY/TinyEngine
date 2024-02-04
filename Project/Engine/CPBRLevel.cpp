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

    // UI ¸¸ ·»´õ¸µ
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
}

void CPBRLevel::tick()
{
    CLevel::tick();
}

void CPBRLevel::finaltick()
{
    CLevel::finaltick();
}
