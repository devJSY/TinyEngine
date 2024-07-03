#include "pch.h"
#include "CKirbyStuffed.h"

CKirbyStuffed::CKirbyStuffed()
{
}

CKirbyStuffed::~CKirbyStuffed()
{
}

void CKirbyStuffed::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        if (PLAYERCTRL->GetInput().Length() != 0.f)
        {
            //ChageState(L"STUFFED_RUN")
        }
        else
        {
            // ChageState(L"STUFFED_IDLE")
        }
    }
}

void CKirbyStuffed::Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Stuffed"));


    
    // ===============================

    PLAYER->Animator()->Play(KIRBYANIM(L"StuffedWait"));

    // Run
    //PLAYER->Animator()->Play(KIRBYANIM(L"StuffedRun"));

    // ===============================

    // clean up vacuum state
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, 0);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 6);
    PLAYERFSM->SetCharge(ChargeType::NONE);
    // @TODO 속도조절

    // enter stuffed state
    PLAYER->Animator()->Play(KIRBYANIM(L"Stuffed"), false);
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetStuffed(true);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 1);
}

void CKirbyStuffed::Exit()
{
    //PLAY_CURSTATE(StuffedExit)
}