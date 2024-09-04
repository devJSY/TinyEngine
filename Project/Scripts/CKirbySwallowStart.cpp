#include "pch.h"
#include "CKirbySwallowStart.h"

#include "CCameraController.h"
#include "CLevelFlowMgr.h"

CKirbySwallowStart::CKirbySwallowStart()
{
}

CKirbySwallowStart::~CKirbySwallowStart()
{
}

void CKirbySwallowStart::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        PLAYERFSM->SetGlobalState(false);
        ChangeState(L"SWALLOW_END");
    }
}

void CKirbySwallowStart::Enter()
{
    // 변신 중일 땐 커비가 이미시브 효과를 받지않도록 한다.
    PLAYERFSM->SetSkrr(true);
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);

    // 커비 SwallowStart 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("SwallowStart"), false, false, 2.f);

    // MoveController Lock
    PLAYERCTRL->LockInput();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    // 커비를 제외한 모든 오브젝트가 멈추도록 타임 스케일을 조절
    CTimeMgr::GetInst()->SetTimeScale(0.f);
    PLAYERCTRL->Animator()->SetAnimatorUpdateMode(AnimatorUpdateMode::UnscaledTime);

    // CameraCtrl
    CCameraController* CamCtrl = CAMERACTRL;
    CamCtrl->SaveSetting();

    Vec3 CamPos = CamCtrl->GetOwner()->Transform()->GetWorldPos();

    // 커비 방향 설정
    Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
    Vec3 Dir = CamPos - PlayerPos;
    Dir.y = 0.f;
    Dir.Normalize();

    ForceDirInfo DirInfo = {ForceDirType::STAGEEVENT, Dir, false};
    PLAYERCTRL->ForceDir(DirInfo);

    // CameraSetting
    Vec3 DirToKirby = -Dir;
    DirToKirby.y = -0.577f;
    DirToKirby.Normalize();

    CamCtrl->SetOffset(Vec3(0.f, 15.f, 0));
    CamCtrl->SetTargetOffset(Vec3(0.f, 0.f, 0.f));
    CamCtrl->SetLookDir(DirToKirby);
    CamCtrl->SetLookDist(250.f);

    CamCtrl->SetZoomMinSpeed(100.f);
    CamCtrl->SetZoomMaxSpeed(500.f);
    CamCtrl->SetZoomThreshold(100.f);

    // Normal Setup으로 변경
    PLAYERFSM->SaveLastCameraSetup((UINT)CamCtrl->GetCameraSetup());
    CamCtrl->SetCameraSetup(CameraSetup::NORMAL);

    // 배경 블러 효과
    CGameObject* Manager = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");
    CLevelFlowMgr* FLowMgrScript = Manager->GetScript<CLevelFlowMgr>();

    if (FLowMgrScript != nullptr)
    {
        FLowMgrScript->OnDimensionFade(0.3f);
    }

}

void CKirbySwallowStart::Exit()
{
    PLAYERFSM->SetSkrr(false);
    PLAYERFSM->SetInvincible(false);

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


}
