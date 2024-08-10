#include "pch.h"
#include "CKirbyChangeObject.h"

#include "CCameraController.h"

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

    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;
    CamCtrl->SaveSetting();

    CamCtrl->SetOffset(Vec3(0.f, 15.f, 0));
    CamCtrl->SetLookDir(Vec3(0.f, -0.35f, 0.937f));
    CamCtrl->SetLookDist(170.f);

    // @TODO 카메라 방향 설정
    Vec3 CamPos = CamCtrl->GetOwner()->Transform()->GetWorldPos();

    // @TODO 커비 방향 설정
    Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
    Vec3 Dir = CamPos - PlayerPos;
    Dir.y = 0.f;
    Dir.Normalize();

    ForceDirInfo DirInfo = {ForceDirType::STAGEEVENT, Dir, false};
    PLAYERCTRL->ForceDir(DirInfo);

    // @TODO 타임 스케일 조정 


    // @TODO
    // 배경 블러 효과
    // 커비를 제외한 모든 오브젝트가 멈추어야 함
}

void CKirbyChangeObject::Exit()
{
    PLAYERFSM->GetCurObject()->ChangeObjectExit();

    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;
    CamCtrl->LoadSetting();

    //@TODO
    // 배경 블러 효과 복구
    // 커비를 의외의 오브젝트 다시 DT받도록 수정
}
