#include "pch.h"
#include "CKirbyChangeAbility.h"

#include "CCameraController.h"

#include "CLevelFlowMgr.h"

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

    if (!m_StarEffect && CHECK_ANIMFRM(GetOwner(), 19))
    {
        GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Skrr.wav", 1, KIRBY_EFFECTSOUND * 1.5f);

        CGameObject* pStarEffect =
            CAssetMgr::GetInst()
                ->Load<CPrefab>(L"prefab\\Effect_KirbyChangeAbilityStarSpawn.pref", L"prefab\\Effect_KirbyChangeAbilityStarSpawn.pref")
                ->Instantiate();

        GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());

        m_StarEffect = true;
    }

    // 애니메이션 재생중 필요한 작업
    PLAYERFSM->GetCurAbility()->ChangeAbility();

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
    CGameObject* Smoke = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\LandingSmokeSpawner.pref")->Instantiate();
    Smoke->Transform()->SetWorldPos(PLAYER->Transform()->GetWorldPos());
    GamePlayStatic::SpawnGameObject(Smoke, Smoke->GetLayerIdx());

    m_StarEffect = false;

    PLAYERFSM->ChangeNextAbility();

    m_bFrameEnter = false;

    // 변신 중일 땐 커비가 이미시브 효과를 받지않도록 한다.
    PLAYERFSM->SetSkrr(true);
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);

    // 소켓 꽂아주기
    PLAYERFSM->GetCurAbility()->ChangeAbilityEnter();

    // 커비 변신 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("CopyFirst"), false, false, 1.f);

    // 커버 머터리얼 다시 설정
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthSmileOpen));
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

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

void CKirbyChangeAbility::Exit()
{
    CTimeMgr::GetInst()->SetTimeScale(1.f);

    // MoveController Lock
    PLAYERCTRL->UnlockInput();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();

    PLAYERFSM->GetCurAbility()->ChangeAbilityExit();

    // 커비 표정 복구
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    // CameraSetting
    CCameraController* CamCtrl = CAMERACTRL;
    CamCtrl->LoadSetting();
    CamCtrl->SetCameraSetup((CameraSetup)PLAYERFSM->GetLastCameraSetup());

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
