#include "CCreateTestLevel.h"

#include <Engine\\global.h>
#include <Engine\\func.h>
#include <Engine\\CLevel.h>
#include <Engine\\CLayer.h>
#include <Engine\\CGameObject.h>
#include <Engine\\components.h>
#include <Engine\\CCollisionMgr.h>
#include <Engine\\CLevelMgr.h>

#include <Scripts\\CPlayerScript.h>

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

    // UI 만 렌더링
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
    pPlayer->AddComponent(new CAnimator2D);
    pPlayer->AddComponent(new CCollider2D);
    pPlayer->AddComponent(new CPlayerScript);

    pPlayer->Transform()->SetAbsolute(true);
    pPlayer->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pPlayer->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

    pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

    pCurLevel->AddObject(pPlayer, L"Player");

    // 광원 추가
    CGameObject* pLight = new CGameObject;
    pLight->SetName(L"Light 1");
    pLight->AddComponent(new CTransform);
    pLight->AddComponent(new CMeshRender);
    pLight->AddComponent(new CLight2D);

    pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

    pCurLevel->AddObject(pLight, L"Light");

    // 충돌 설정
    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        for (UINT j = 0; j <= i; j++)
        {
            CCollisionMgr::GetInst()->LayerCheck(i, j);
        }
    }

    GamePlayStatic::ChangeLevel(pCurLevel, LEVEL_STATE::STOP);
}
