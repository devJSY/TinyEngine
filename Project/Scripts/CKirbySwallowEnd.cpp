#include "pch.h"
#include "CKirbySwallowEnd.h"

#include "CCameraController.h"
#include "CLevelFlowMgr.h"

CKirbySwallowEnd::CKirbySwallowEnd()
{
}

CKirbySwallowEnd::~CKirbySwallowEnd()
{
}

void CKirbySwallowEnd::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"CHANGE_ABILITY");
    }

}

void CKirbySwallowEnd::Enter()
{
    // 변신 중일 땐 커비가 이미시브 효과를 받지않도록 한다.
    PLAYERFSM->SetSkrr(true);
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);

    // 커비 SwallowEnd 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("SwallowStart"), false, false, 2.f);

    // MoveController Lock
    PLAYERCTRL->LockInput();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    // 커비를 제외한 모든 오브젝트가 멈추도록 타임 스케일을 조절
    CTimeMgr::GetInst()->SetTimeScale(0.f);
    PLAYERCTRL->Animator()->SetAnimatorUpdateMode(AnimatorUpdateMode::UnscaledTime);

    // 배경 블러 효과
    CGameObject* Manager = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");
    CLevelFlowMgr* FLowMgrScript = Manager->GetScript<CLevelFlowMgr>();

    if (FLowMgrScript != nullptr)
    {
        FLowMgrScript->OnDimensionFade(0.3f);
    }

}

void CKirbySwallowEnd::Exit()
{
    // MoveController Lock
    PLAYERCTRL->UnlockInput();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();

    // 배경 블러 효과 복구
    CGameObject* Manager = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");
    CLevelFlowMgr* FLowMgrScript = Manager->GetScript<CLevelFlowMgr>();

    if (FLowMgrScript != nullptr)
    {
        FLowMgrScript->OffDimensionFade();
    }

    // 타임 스케일 조정
    CTimeMgr::GetInst()->SetTimeScale(1.f);
    PLAYERCTRL->Animator()->SetAnimatorUpdateMode(AnimatorUpdateMode::Normal);

    // Emissive를 다시 받도록 수정
    PLAYERFSM->SetSkrr(false);
    PLAYERFSM->SetInvincible(false);

}
