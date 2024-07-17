#include "pch.h"
#include "CKirbyAbility_Cutter.h"

#include "CKirbyHatBlade.h"

CKirbyAbility_Cutter::CKirbyAbility_Cutter()
{
}

CKirbyAbility_Cutter::~CKirbyAbility_Cutter()
{
}

void CKirbyAbility_Cutter::Attack()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CKirbyAbility_Cutter::AttackEnter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"MetalCutterThrow"), false, false, 1.5f);

    CKirbyHatBlade* HatBladeScript = dynamic_cast<CKirbyHatBlade*>(PLAYERFSM->GetCurHatBlade()->GetScripts()[0]);

    if (HatBladeScript != nullptr)
    {
        HatBladeScript->Attack();
    }
}

void CKirbyAbility_Cutter::AttackExit()
{
}

void CKirbyAbility_Cutter::ChangeAbility()
{
}

void CKirbyAbility_Cutter::ChangeAbilityEnter()
{
    // 소켓에 모자 끼워주기
    CGameObject* pInstObj = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\MetalHat.pref", L"prefab\\MetalHat.pref")->Instantiate();
    GamePlayStatic::AddChildObject(PLAYER, pInstObj, L"Hat");

    // FSM에 모자 등록
    PLAYERFSM->SetCurHat(pInstObj);

    // HatBlade 따로 생성후 끼워주기
    pInstObj = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\MetalHatBlade.pref", L"prefab\\MetalHatBlade.pref")->Instantiate();
    GamePlayStatic::AddChildObject(PLAYER, pInstObj, L"Hat");

    // FSM에 HatBlade 저장
    PLAYERFSM->SetCurHatBlade(pInstObj);
    PLAYERFSM->SetBladeAttack(true);

    // 애니메이션 재생
    pInstObj->Animator()->Play(KIRBYANIM(L"BladeWait"));

}

void CKirbyAbility_Cutter::ChangeAbilityExit()
{
}
