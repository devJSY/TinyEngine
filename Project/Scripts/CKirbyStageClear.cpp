#include "pch.h"
#include "CKirbyStageClear.h"

#include "CCameraController.h"

#include "CLevelFlowMgr.h"

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

        // CameraSetting
        CCameraController* CamCtrl = CAMERACTRL;
        CamCtrl->SetMainTarget(PLAYER);
        CamCtrl->SetLookDir(-PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT));
        CamCtrl->SetOffset(Vec3(0.f, 50.f, 0));
        CamCtrl->SetTargetOffset(Vec3(0.f, 0.f, 0.f));
        CamCtrl->SetLookDist(300.f);
        CamCtrl->RotationLookDirRightAxis(5.f);
        CamCtrl->Normal(true);


        CamCtrl->SetOffset(Vec3(0.f, -5.f, 0));
        CamCtrl->RotationLookDirRightAxis(10.f);
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

        m_Step = 20;
    }

    if (m_Step == 20 && CHECK_ANIMFRM(GetOwner(), 299))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Frown);

        m_Step = 21;
    }

    if (m_Step == 21 && CHECK_ANIMFRM(GetOwner(), 316))
    {
        CPlayerMgr::SetPlayerFace(FaceType::Normal);
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthSmileOpen);

        Vec3 LookDir = -PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        LookDir.y = 0.f;
        LookDir.Normalize();

        CCameraController* CamCtrl = CAMERACTRL;
        CamCtrl->SetOffset(Vec3(0.f, 45.f, 0));
        CamCtrl->SetLookDir(LookDir);
        CamCtrl->SetLookDist(150.f);

        CamCtrl->SetRotationSpeed(100.f);
        CamCtrl->SetMinSpeed(50.f);
        CamCtrl->SetMaxSpeed(300.f);
        CamCtrl->SetThresholdDistance(50.f);

        CamCtrl->SetZoomMinSpeed(50.f);
        CamCtrl->SetZoomMaxSpeed(300.f);
        CamCtrl->SetZoomThreshold(150.f);

        // StageClear UI On
        CGameObject* ManagerObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");

        CLevelFlowMgr* FlowMgrScript = ManagerObj->GetScript<CLevelFlowMgr>();

        FlowMgrScript->TurnOnStageclearUI();

        m_Step = 22;
    }

    if (m_Step == 22 && KEY_TAP(KEY_ATK))
    {
        // Level Change
        CGameObject* Manager = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");

        if (Manager != nullptr)
        {
            CLevelFlowMgr* FlowMgr = Manager->GetScript<CLevelFlowMgr>();
            if (FlowMgr != nullptr)
            {
                FlowMgr->LevelEnd();
            }
        }
    }
}

void CKirbyStageClear::Enter()
{
    // @TODO StageClear Sound

    CAMERACTRL->TurnOffMonsterLayer();

    // MoveController Lock
    PLAYERCTRL->LockInput();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    // 커비 표정 세팅
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(CPlayerMgr::GetPlayerMeshIdx().MouthSmileClose);

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("ClearDanceLong"), false, false, 1.7f);

    // UI 다 끄기
    CGameObject* ManagerObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");

    // Camera 속성을 초기값으로 돌린다.
    CAMERACTRL->LoadInitSetting(true);

    CLevelFlowMgr* FlowMgrScript = ManagerObj->GetScript<CLevelFlowMgr>();
    FlowMgrScript->TurnOffBossHP();
    FlowMgrScript->TurnOffPlayerHP();
    FlowMgrScript->ActiveOffDropUI();

    m_Step = 0;
    m_Duration = 2.f;
    m_Acc = 0.f;
}

void CKirbyStageClear::Exit()
{
}
