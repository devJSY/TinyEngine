#include "pch.h"
#include "CTestLevel2.h"

#include "CCollisionMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "components.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

#include "CScriptMgr.h"

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

    pCamObj->Camera()->SetCameraPriority(1);
    pCamObj->Camera()->LayerCheck(L"UI", true);

    AddObject(pCamObj, L"Camera");

    // AnimObj
    CGameObject* pPlayer = new CGameObject;
    pPlayer->SetName(L"Player");

    pPlayer->AddComponent(new CTransform);
    pPlayer->AddComponent(new CMeshRender);
    pPlayer->AddComponent(new CCollider2D);
    pPlayer->AddComponent(new CAnimator2D);
    pPlayer->AddComponent(CScriptMgr::GetScript(SCRIPT_TYPE::PLAYERSCRIPT));

    pPlayer->Transform()->SetAbsolute(true);
    pPlayer->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    pPlayer->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

    pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

    //Ptr<CTexture> Convict_Tex =
    //    CAssetMgr::GetInst()->Load<CTexture>(L"Convict", L"texture//Player//Character//Convict//Convict.png");

    //Vec2 vLT = Vec2(0.f, 0.f);
    //Vec2 vSlice = Vec2(32.f, 32.f);
    //Vec2 vOffset = Vec2(0.f, 0.f);
    //Vec2 vBG = Vec2(64.f, 64.f);
    //float fFPS = 12.f;

    //pPlayer->Animator2D()->Create(L"dodge_back", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"dodge_backward", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"dodge_forward", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);
    //pPlayer->Animator2D()->Create(L"dodge_front", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"idle_back", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"idle_back_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"idle_backward", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"idle_forward", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"idle_forward_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"idle_front", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"idle_front_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"run_back", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"run_back_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"run_backward", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"run_backward_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"run_doorway", Convict_Tex, vLT, vSlice, vOffset, vBG, 7, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"run_forward", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"run_forward_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"run_front", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"run_front_hands", Convict_Tex, vLT, vSlice, vOffset, vBG, 6, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"select_choose", Convict_Tex, vLT, vSlice, vOffset, vBG, 9, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"select_idle", Convict_Tex, vLT, vSlice, vOffset, vBG, 5, fFPS, false);
    //vLT += Vec2(0.0f, 32.f);

    //pPlayer->Animator2D()->Create(L"select_smoking_idle", Convict_Tex, vLT, vSlice, vOffset, vBG, 4, fFPS, false);

    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_back.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_backward.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_forward.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\dodge_front.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_back.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_back_hands.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_backward.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_forward.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_forward_hands.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_front.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\idle_front_hands.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_back.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_back_hands.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_backward.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_backward_hands.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_doorway.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_forward.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_forward_hands.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_front.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\run_front_hands.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\select_choose.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\select_idle.anim");
    pPlayer->Animator2D()->LoadAnimation(L"AnimData\\Player\\Convict\\select_smoking_idle.anim");

    pPlayer->Animator2D()->Play(L"dodge_back", true);

    AddObject(pPlayer, L"Player");

    CGameObject* pTest = new CGameObject;
    pTest->SetName(L"Test");

    pTest->AddComponent(new CTransform);
    pTest->AddComponent(new CMeshRender);

    pTest->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    pTest->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
    pTest->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"missing_texture"));

    AddObject(pTest, L"Default");


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
