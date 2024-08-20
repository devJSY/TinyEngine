#include "pch.h"
#include "CKirbyChangeObject.h"

#include "CCameraController.h"
#include "CLevelFlowMgr.h"

CKirbyChangeObject::CKirbyChangeObject()
{
}

CKirbyChangeObject::~CKirbyChangeObject()
{
}

void CKirbyChangeObject::tick()
{
    PLAYERFSM->GetCurObject()->ChangeObject();

    // State Change
    switch (PLAYERFSM->GetCurObjectIdx())
    {
    case ObjectCopyType::CONE:
    case ObjectCopyType::VENDING_MACHINE:
    case ObjectCopyType::LIGHT: {
        if (PLAYER->Animator()->IsFinish())
        {
            PLAYERFSM->SetGlobalState(false);
            ChangeState(L"CHANGE_OBJECT_END");
        }
    }
        break;
    }
}

void CKirbyChangeObject::Enter()
{
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->GetCurObject()->ChangeObjectEnter();

    // 변신 중일 땐 커비가 이미시브 효과를 받지않도록 한다.
    PLAYERFSM->SetSkrr(true);

    // CameraSetting
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
    CamCtrl->SetLookDir(DirToKirby);
    CamCtrl->SetLookDist(250.f);

    // Normal Setup으로 변경
    m_SaveSetup = (UINT)CamCtrl->GetCameraSetup();
    CamCtrl->SetCameraSetup(CameraSetup::NORMAL);

    // 배경 블러 효과
    CGameObject* Manager = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");
    CLevelFlowMgr* FLowMgrScript = Manager->GetScript<CLevelFlowMgr>();

    if (FLowMgrScript != nullptr)
    {
        FLowMgrScript->OnDimensionFade(0.3f);
    }

    // 커비를 제외한 모든 오브젝트가 멈추도록 타임 스케일을 조절
    CTimeMgr::GetInst()->SetTimeScale(0.f);
    PLAYERCTRL->Animator()->SetAnimatorUpdateMode(AnimatorUpdateMode::UnscaledTime);

}

void CKirbyChangeObject::Exit()
{
    PLAYERFSM->GetCurObject()->ChangeObjectExit();

    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;
    CamCtrl->LoadSetting();

    CamCtrl->SetCameraSetup((CameraSetup)m_SaveSetup);

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
}
