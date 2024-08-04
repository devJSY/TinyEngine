#include "pch.h"
#include "CKirbyDeath.h"

#include "CCameraController.h"

CKirbyDeath::CKirbyDeath()
{
}

CKirbyDeath::~CKirbyDeath()
{
}

void CKirbyDeath::tick()
{
    m_Acc += DT;

    if (m_Acc != 0.f)
    {
        PLAYER->Animator()->SetPlay(false);
    }

    // 멈추는 시간이 끝난경우
    if (m_Acc > m_Duration)
    {
        // 애니메이션 재생
        PLAYER->Animator()->SetPlay(true);

        // @TODO 커비와 UI를 제외한 배경은 검은색으로 Fade Out (마스킹된 커비의 위치를 제외한 모든곳을 점점 검은색으로 바꿔주는 Post Process 쉐이더 적용하면 될 듯)


        
    }

    if (m_Acc > m_FaceDuraion)
    {
        CPlayerMgr::SetPlayerFace(FaceType::Close); 

        // Camera 조작
        CCameraController* CamCtrl = CAMERACTRL;
        CamCtrl->SetOffset(Vec3(0.f, 0.f, 0));
        CamCtrl->SetLookDir(Vec3(0.f, -1.f, 0.f));
        CamCtrl->SetLookDist(100.f);
        CamCtrl->SetZoomMinSpeed(50.f);
        CamCtrl->SetZoomMaxSpeed(100.f);
        CamCtrl->SetZoomThreshold(500.f);
    }

    if (m_Acc > m_DeathDuraion)
    {
        // @TODO 애니메이션이 끝나면 UI가 화면을 가림

        // @TODO UI애니메이션 모두 끝났다면 커비가 되살아 남

    }
}

void CKirbyDeath::Enter()
{
    // @TODO m_Duration 만큼 시간을 멈추기

    // 커비 표정
    CPlayerMgr::SetPlayerFace(FaceType::Frown);

    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;

    // Camera Shake
    CamCtrl->Shake(0.5f, 30.f, 30.f);
     
    Vec3 CamPos = CamCtrl->GetOwner()->Transform()->GetWorldPos();

    // @TODO 커비 방향 설정
    Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
    Vec3 Dir = CamPos - PlayerPos;
    Dir.y = 0.f;
    Dir.Normalize();

    ForceDirInfo DirInfo = {ForceDirType::STAGEEVENT, Dir,true};
    PLAYERCTRL->ForceDir(DirInfo);
    PLAYERFSM->SetGlobalState(true);
    GetOwner()->Animator()->Play(ANIMPREFIX("Death"), false);

    PLAYERCTRL->LockInput();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();

    m_Acc = 0.f;
    m_Duration = 1.f;
    m_FaceDuraion = 3.f;
    m_DeathDuraion = 6.f;
}

void CKirbyDeath::Exit()
{
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
}