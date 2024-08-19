#include "pch.h"
#include "CKirbyChangeAbilityEnd.h"

#include "CCameraController.h"

CKirbyChangeAbilityEnd::CKirbyChangeAbilityEnd()
{
}

CKirbyChangeAbilityEnd::~CKirbyChangeAbilityEnd()
{
}

void CKirbyChangeAbilityEnd::tick()
{
    // State Change

    switch (PLAYERFSM->GetCurAbilityIdx())
    {
    case AbilityCopyType::FIRE:
    case AbilityCopyType::NORMAL:
    case AbilityCopyType::CUTTER:
    case AbilityCopyType::SWORD: {
        if (PLAYER->Animator()->IsFinish())
        {
            PLAYERFSM->SetGlobalState(false);
            ChangeState(L"IDLE");
        }
    }
    break;
    case AbilityCopyType::SLEEP: {
        if (PLAYER->Animator()->IsFinish())
        {
            PLAYERFSM->SetGlobalState(false);
            ChangeState(L"IDLE_START");
        }
    }
    break;
    }
}

void CKirbyChangeAbilityEnd::Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("EvolutionCopyEnd"), false, false, 1.5f);

    // MoveController Lock
    PLAYERCTRL->LockInput();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);
}

void CKirbyChangeAbilityEnd::Exit()
{
    // 커비 표정 복구
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;
    CamCtrl->LoadSetting();

    // MoveController Lock
    PLAYERCTRL->UnlockInput();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();

    PLAYERFSM->SetInvincible(false);

    //@TODO
    // 배경 블러 효과 복구
    // 커비를 의외의 오브젝트 다시 DT받도록 수정
}
