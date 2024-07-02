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
        PLAYERFSM->SetGlobalState(false);
        ChangeState(L"IDLE");
    }
}

void CKirbyStuffed::Enter()
{
    // clean up vacuum state
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, 0);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 6);
    PLAYERFSM->SetVacuum(false);
    // @TODO 속도조절

    // enter stuffed state
    PLAYER->Animator()->Play(KIRBYANIM(L"Stuffed"), false);
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetStuffed(true);
    PLAYER->GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\BodyC.mtrl"), 1);
}

void CKirbyStuffed::Exit()
{
}