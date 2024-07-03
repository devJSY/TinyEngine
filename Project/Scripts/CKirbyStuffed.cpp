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
    // PLAY_CURSTATE(Stuffed)

    if (PLAYER->Animator()->IsFinish())
    {
        if (PLAYERCTRL->GetInput().Length() != 0.f)
        {
            ChangeState(L"STUFFED_RUN");
        }
        else
        {
            ChangeState(L"STUFFED_IDLE");
        }
    }
}

void CKirbyStuffed::Enter()
{
    // PLAY_CURSTATE(StuffedEnter)
     
    // clean up vacuum state
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, 0);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 6);
    // @TODO 속도조절

    // enter stuffed state
    PLAYER->Animator()->Play(KIRBYANIM(L"Stuffed"), false);
    PLAYERFSM->SetStuffed(true);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 1);
}

void CKirbyStuffed::Exit()
{
    // PLAY_CURSTATE(StuffedExit)
}