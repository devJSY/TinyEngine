#include "pch.h"
#include "CKirbyChangeAbilityEnd.h"

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
    PLAYERFSM->SetGlobalState(true);
    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("EvolutionCopyEnd"), false, false, 1.5f);
}

void CKirbyChangeAbilityEnd::Exit()
{
    // 커비 표정 복구
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    //@TODO
    // 카메라 원상 복구
    // 배경 블러 효과 복구
    // 커비 움직임 제한 복구
    // 커비를 의외의 오브젝트 다시 DT받도록 수정
}
