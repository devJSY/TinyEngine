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

    Ptr<CTexture> dodge_back_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"dodge_back", L"texture//Player//Character//Convict//dodge_back//dodge_back.png");
    Ptr<CTexture> dodge_backward_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"dodge_backward", L"texture//Player//Character//Convict//dodge_backward//dodge_backward.png");
    Ptr<CTexture> dodge_forward_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"dodge_forward", L"texture//Player//Character//Convict//dodge_forward//dodge_forward.png");
    Ptr<CTexture> dodge_front_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"dodge_front", L"texture//Player//Character//Convict//dodge_front//dodge_front.png");
    Ptr<CTexture> idle_back_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"idle_back", L"texture//Player//Character//Convict//idle_back//idle_back.png");
    Ptr<CTexture> idle_back_hands_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"idle_back_hands", L"texture//Player//Character//Convict//idle_back_hands//idle_back_hands.png");
    Ptr<CTexture> idle_backward_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"idle_backward", L"texture//Player//Character//Convict//idle_backward//idle_backward.png");
    Ptr<CTexture> idle_forward_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"idle_forward", L"texture//Player//Character//Convict//idle_forward//idle_forward.png");
    Ptr<CTexture> idle_forward_hands_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"idle_forward_hands", L"texture//Player//Character//Convict//idle_forward_hands//idle_forward_hands.png");
    Ptr<CTexture> idle_front_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"idle_front", L"texture//Player//Character//Convict//idle_front//idle_front.png");
    Ptr<CTexture> idle_front_hands_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"idle_front_hands", L"texture//Player//Character//Convict//idle_front_hands//idle_front_hands.png");
    Ptr<CTexture> run_back_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"run_back", L"texture//Player//Character//Convict//run_back//run_back.png");
    Ptr<CTexture> run_back_hands_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"run_back_hands", L"texture//Player//Character//Convict//run_back_hands//run_back_hands.png");
    Ptr<CTexture> run_backward_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"run_backward", L"texture//Player//Character//Convict//run_backward//run_backward.png");
    Ptr<CTexture> run_backward_hands_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"run_backward_hands", L"texture//Player//Character//Convict//run_backward_hands//run_backward_hands.png");
    Ptr<CTexture> run_doorway_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"run_doorway", L"texture//Player//Character//Convict//run_doorway//run_doorway.png");
    Ptr<CTexture> run_forward_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"run_forward", L"texture//Player//Character//Convict//run_forward//run_forward.png");
    Ptr<CTexture> run_forward_hands_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"run_forward_hands", L"texture//Player//Character//Convict//run_forward_hands//run_forward_hands.png");
    Ptr<CTexture> run_front_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"run_front", L"texture//Player//Character//Convict//run_front//run_front.png");
    Ptr<CTexture> run_front_hands_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"run_front_hands", L"texture//Player//Character//Convict//run_front_hands//run_front_hands.png");
    Ptr<CTexture> select_choose_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"select_choose", L"texture//Player//Character//Convict//select_choose//select_choose.png");
    Ptr<CTexture> select_idle_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"select_idle", L"texture//Player//Character//Convict//select_idle//select_idle.png");
    Ptr<CTexture> select_smoking_idle_Tex = CAssetMgr::GetInst()->Load<CTexture>(
        L"select_smoking_idle", L"texture//Player//Character//Convict//select_smoking_idle//select_smoking_idle.png");

    Vec2 vLT = Vec2(0.f, 0.f);
    Vec2 vSlice = Vec2(32.f, 32.f);
    Vec2 vOffset = Vec2(0.f, 0.f);
    Vec2 vBG = Vec2(64.f, 64.f);
    float fFPS = 12.f;

    pPlayer->Animator2D()->Create(L"dodge_back", dodge_back_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    pPlayer->Animator2D()->Create(L"dodge_backward", dodge_backward_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    pPlayer->Animator2D()->Create(L"dodge_forward", dodge_forward_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    pPlayer->Animator2D()->Create(L"dodge_front", dodge_front_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    pPlayer->Animator2D()->Create(L"idle_back", idle_back_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    pPlayer->Animator2D()->Create(L"idle_back_hands", idle_back_hands_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    pPlayer->Animator2D()->Create(L"idle_backward", idle_backward_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    pPlayer->Animator2D()->Create(L"idle_forward", idle_forward_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    pPlayer->Animator2D()->Create(L"idle_forward_hands", idle_forward_hands_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS,
                                  false);
    pPlayer->Animator2D()->Create(L"idle_front", idle_front_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    pPlayer->Animator2D()->Create(L"idle_front_hands", idle_front_hands_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    pPlayer->Animator2D()->Create(L"run_back", run_back_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    pPlayer->Animator2D()->Create(L"run_back_hands", run_back_hands_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    pPlayer->Animator2D()->Create(L"run_backward", run_backward_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    pPlayer->Animator2D()->Create(L"run_backward_hands", run_backward_hands_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS,
                                  false);
    pPlayer->Animator2D()->Create(L"run_doorway", run_doorway_Tex, vLT, vSlice, vOffset, vBG, 7, fFPS, false);
    pPlayer->Animator2D()->Create(L"run_forward", run_forward_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    pPlayer->Animator2D()->Create(L"run_forward_hands", run_forward_hands_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS,
                                  false);
    pPlayer->Animator2D()->Create(L"run_front", run_front_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    pPlayer->Animator2D()->Create(L"run_front_hands", run_front_hands_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    pPlayer->Animator2D()->Create(L"select_choose", select_choose_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    pPlayer->Animator2D()->Create(L"select_idle", select_idle_Tex, vLT, vSlice, vOffset, vBG, 5, fFPS, false);
    pPlayer->Animator2D()->Create(L"select_smoking_idle", select_smoking_idle_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS,
                                  false);

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
