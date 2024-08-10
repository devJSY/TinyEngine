#include "pch.h"
#include "CKirbyChangeAbility.h"

#include "CCameraController.h"

CKirbyChangeAbility::CKirbyChangeAbility()
{
}

CKirbyChangeAbility::~CKirbyChangeAbility()
{
}

void CKirbyChangeAbility::tick()
{
    if (!m_bFrameEnter && CHECK_ANIMFRM(GetOwner(), 0))
    {
        PLAYER->Animator()->SetPlay(false);
        m_bFrameEnter = true;
    }
    else
    {
        PLAYER->Animator()->SetPlay(true);
    }

    // 애니메이션 재생중 필요한 작업 
    PLAYERFSM->GetNextAbility()->ChangeAbility();

    if (PLAYER->Animator()->IsFinish())
    {
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

}

void CKirbyChangeAbility::Enter()
{
    m_bFrameEnter = false;

    PLAYERFSM->SetGlobalState(true);

    // 소켓 꽂아주기
    PLAYERFSM->GetNextAbility()->ChangeAbilityEnter();
    
    // 커비 변신 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("CopyFirst"), false, false, 1.5f);

    // 커버 머터리얼 다시 설정
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthSmileOpen));
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    // MoveController Lock
    PLAYERCTRL->LockInput();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;
    CamCtrl->SaveSetting();

    CamCtrl->SetOffset(Vec3(0.f, 15.f, 0));
    CamCtrl->SetLookDir(Vec3(0.f, -0.35f, 0.937f));
    CamCtrl->SetLookDist(170.f);

    // @TODO 타임 스케일 조정 

    // @TODO 카메라 방향 설정
    Vec3 CamPos = CamCtrl->GetOwner()->Transform()->GetWorldPos();

    // @TODO 커비 방향 설정
    Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
    Vec3 Dir = CamPos - PlayerPos;
    Dir.y = 0.f;
    Dir.Normalize();

    ForceDirInfo DirInfo = {ForceDirType::STAGEEVENT, Dir, false};
    PLAYERCTRL->ForceDir(DirInfo);

    // @TODO
    // 배경 블러 효과
    // 커비를 제외한 모든 오브젝트가 멈추어야 함
}

void CKirbyChangeAbility::Exit()
{
    // MoveController Lock
    PLAYERCTRL->UnlockInput();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();

    PLAYERFSM->GetNextAbility()->ChangeAbilityExit();

    // 커비 표정 복구
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;
    CamCtrl->LoadSetting();

    //@TODO
    // 배경 블러 효과 복구
    // 커비를 의외의 오브젝트 다시 DT받도록 수정
}
