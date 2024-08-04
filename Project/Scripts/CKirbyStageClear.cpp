#include "pch.h"
#include "CKirbyStageClear.h"

#include "CCameraController.h"

CKirbyStageClear::CKirbyStageClear()
{
}

CKirbyStageClear::~CKirbyStageClear()
{
}

void CKirbyStageClear::tick()
{
    m_Acc += DT;

    // 방향, 카메라 세팅
    if (m_Step == 0 && CHECK_ANIMFRM(GetOwner(), 0))
    {
        PLAYER->Animator()->SetPlay(false);

        CCameraController* CamCtrl = CAMERACTRL;
        CamCtrl->SetOffset(Vec3(0.f, -5.f, 0));
        CamCtrl->SetLookDir(Vec3(0.f, -0.331f, 0.944f));
        CamCtrl->SetLookDist(200.f);

        m_Step = 1;
    }

    if (m_Step == 1 && m_Acc > m_Duration)
    {
        PLAYER->Animator()->SetPlay(true);
        CPlayerMgr::SetPlayerFace(FaceType::Frown);

        m_Step = 2;
    }

    if (m_Step == 2 && CHECK_ANIMFRM(GetOwner(), 24))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Normal);
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthNormal);

        m_Step = 3;
    }

    if (m_Step == 3 && CHECK_ANIMFRM(GetOwner(), 48))
    {
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthSmileClose);

        m_Step = 4;
    }

    if (m_Step == 4 && CHECK_ANIMFRM(GetOwner(), 93))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Half);

        m_Step = 5;
    }

    if (m_Step == 5 && CHECK_ANIMFRM(GetOwner(), 94))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Close);

        m_Step = 6;
    }

    if (m_Step == 6 && CHECK_ANIMFRM(GetOwner(), 99))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Half);

        m_Step = 7;
    }

    if (m_Step == 7 && CHECK_ANIMFRM(GetOwner(), 100))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Normal);
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthOpen);

        m_Step = 8;
    }

    if (m_Step == 8 && CHECK_ANIMFRM(GetOwner(), 155))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Frown);
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthSmileClose);

        m_Step = 9;
    }

    if (m_Step == 9 && CHECK_ANIMFRM(GetOwner(), 167))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Half);

        m_Step = 10;
    }

    if (m_Step == 10 && CHECK_ANIMFRM(GetOwner(), 168))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Normal);

        m_Step = 11;
    }

    if (m_Step == 11 && CHECK_ANIMFRM(GetOwner(), 213))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Close);

        m_Step = 12;
    }

    if (m_Step == 12 && CHECK_ANIMFRM(GetOwner(), 218))
    {
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthNormal);

        m_Step = 13;
    }

    if (m_Step == 13 && CHECK_ANIMFRM(GetOwner(), 219))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Normal);

        m_Step = 14;
    }

    if (m_Step == 14 && CHECK_ANIMFRM(GetOwner(), 244))
    {
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthSmileOpen);

        m_Step = 15;
    }

    if (m_Step == 15 && CHECK_ANIMFRM(GetOwner(), 252))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Close);

        m_Step = 16;
    }

    if (m_Step == 16 && CHECK_ANIMFRM(GetOwner(), 253))
    {
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthSmileClose);

        m_Step = 17;
    }

    if (m_Step == 17 && CHECK_ANIMFRM(GetOwner(), 260))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Half);

        m_Step = 18;
    }

    if (m_Step == 18 && CHECK_ANIMFRM(GetOwner(), 261))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Normal);

        m_Step = 19;
    }

    if (m_Step == 19 && CHECK_ANIMFRM(GetOwner(), 298))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Half);

        CCameraController* CamCtrl = CAMERACTRL;
        CamCtrl->SetOffset(Vec3(0.f, 10.f, 0));
        CamCtrl->SetLookDir(Vec3(0.f, -0.216f, 0.976f));
        CamCtrl->SetLookDist(150.f);

        m_Step = 20;
    }

    if (m_Step == 20 && CHECK_ANIMFRM(GetOwner(), 299))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Frown);

        m_Step = 21;
    }

    if (m_Step == 21 && CHECK_ANIMFRM(GetOwner(), 319))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Normal);
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthSmileOpen);

        m_Step = 22;
    }

    if (m_Step == 22 && KEY_TAP(KEY_ATK))
    {
        // @TODO Level Change
        ChangeState(L"IDLE");
    }
}

void CKirbyStageClear::Enter()
{
    // @TODO StageClear Sound

    // MoveController Lock
    PLAYERCTRL->LockInput();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;
    CamCtrl->SetOffset(Vec3(0.f, 50.f, 0));
    CamCtrl->SetLookDir(Vec3(0.f, 0.f, 0.944f));
    CamCtrl->SetLookDist(300.f);

    CamCtrl->ResetCamera();

    // @TODO 카메라 방향 설정
    Vec3 CamPos = CamCtrl->GetOwner()->Transform()->GetWorldPos();

    // @TODO 커비 방향 설정
    Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
    Vec3 Dir = CamPos - PlayerPos;
    Dir.y = 0.f;
    Dir.Normalize();

    ForceDirInfo DirInfo = {ForceDirType::STAGECLEAR, Dir};
    PLAYERCTRL->ForceDir(DirInfo);

    // 커비 표정 세팅
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthSmileClose);

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("ClearDanceLong"), false, false, 1.7f);

    m_Step = 0;
    m_Duration = 2.f;
    m_Acc = 0.f;
}

void CKirbyStageClear::Exit()
{
}
