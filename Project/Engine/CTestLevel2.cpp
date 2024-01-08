#include "pch.h"
#include "CTestLevel2.h"

#include "CCollisionMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CAnimator2D.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

CTestLevel2::CTestLevel2()
{
}

CTestLevel2::~CTestLevel2()
{
}

void CTestLevel2::begin()
{
    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);
    pCamObj->AddComponent(new CCameraMoveScript);

    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerCheckAll();
    pCamObj->Camera()->LayerCheck(L"UI", false);

    AddObject(pCamObj, L"Camera");

    // UI 만 렌더링
    pCamObj = new CGameObject;
    pCamObj->SetName(L"UI Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(1);
    pCamObj->Camera()->LayerCheck(L"UI", true);

    AddObject(pCamObj, L"Camera");

    // AnimObj
    CGameObject* pPlayer = new CGameObject;
    pPlayer->SetName(L"Player");

    pPlayer->AddComponent(new CTransform);
    pPlayer->AddComponent(new CMeshRender);
    pPlayer->AddComponent(new CCollider2D);
    pPlayer->AddComponent(new CPlayerScript);
    pPlayer->AddComponent(new CAnimator2D);

    pPlayer->Transform()->SetAbsolute(true);
    pPlayer->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pPlayer->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

    pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

    Ptr<CTexture> Convict_Tex =
        CAssetMgr::GetInst()->Load<CTexture>(L"Convict", L"texture//Player//Character//Convict//Convict.png");

    Vec2 vLT = Vec2(0.f, 0.f);
    Vec2 vSlice = Vec2(32.f, 32.f);
    Vec2 vOffset = Vec2(0.f, 0.f);
    Vec2 vBG = Vec2(64.f, 64.f);
    float fFPS = 12.f;

    pPlayer->Animator2D()->Create(L"dodge_back", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"dodge_backward", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"dodge_forward", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    vLT += Vec2(0.0f, 32.f);
    pPlayer->Animator2D()->Create(L"dodge_front", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"idle_back", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"idle_back_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"idle_backward", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"idle_forward", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"idle_forward_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"idle_front", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"idle_front_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"run_back", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"run_back_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"run_backward", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"run_backward_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"run_doorway", Convict_Tex, vLT, vSlice, vOffset, vBG, 7, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"run_forward", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"run_forward_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"run_front", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"run_front_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"select_choose", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"select_idle", Convict_Tex, vLT, vSlice, vOffset, vBG, 5, fFPS, false);
    vLT += Vec2(0.0f, 32.f);

    pPlayer->Animator2D()->Create(L"select_smoking_idle", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);

    pPlayer->Animator2D()->Play(L"dodge_back", true);

    AddObject(pPlayer, L"Player");

    // 충돌 설정
    for (UINT i = 0; i < LAYER_MAX; i++)
    {
        for (UINT j = 0; j <= i; j++)
        {
            CCollisionMgr::GetInst()->LayerCheck(i, j);
        }
    }

    CLevel::begin();
}

void CTestLevel2::tick()
{
    CLevel::tick();
}

void CTestLevel2::finaltick()
{
    CLevel::finaltick();
}
