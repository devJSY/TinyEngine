#include "pch.h"
#include "CTestLevel.h"

#include "CCollisionMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "components.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

#include "CScriptMgr.h"

#include "CSetColorShader.h"

CTestLevel::CTestLevel()
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
    GetLayer(13)->SetName(L"BackGround");
    GetLayer(14)->SetName(L"PostProcess");
    GetLayer(15)->SetName(L"UI");

    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);
    pCamObj->AddComponent(CScriptMgr::GetScript(SCRIPT_TYPE::CAMERAMOVESCRIPT));

    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerCheckAll();
    pCamObj->Camera()->LayerCheck(this, L"UI", false);

    AddObject(pCamObj, L"Camera");

    // UI 만 렌더링
    pCamObj = new CGameObject;
    pCamObj->SetName(L"UI Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(1);
    pCamObj->Camera()->LayerCheck(this, L"UI", true);

    AddObject(pCamObj, L"Camera");

    // Player
    CGameObject* pPlayer = new CGameObject;
    pPlayer->SetName(L"Player");

    pPlayer->AddComponent(new CTransform);
    pPlayer->AddComponent(new CMeshRender);
    pPlayer->AddComponent(new CAnimator2D);
    pPlayer->AddComponent(new CCollider2D);
    pPlayer->AddComponent(CScriptMgr::GetScript(SCRIPT_TYPE::PLAYERSCRIPT));

    pPlayer->Transform()->SetAbsolute(true);
    pPlayer->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pPlayer->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

    pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

    AddObject(pPlayer, L"Player");

    // 광원 추가
    CGameObject* pLight = new CGameObject;
    pLight->SetName(L"Light 1");
    pLight->AddComponent(new CTransform);
    pLight->AddComponent(new CMeshRender);
    pLight->AddComponent(new CLight2D);

    pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

    AddObject(pLight, L"Light");

    // 충돌 설정
    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        for (UINT j = 0; j <= i; j++)
        {
            CCollisionMgr::GetInst()->LayerCheck(i, j);
        }
    }
}

CTestLevel::~CTestLevel()
{
}
