#include "pch.h"
#include "CElfilisD_Roar.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CElfilisBigFSM.h"
#include "CBossLevelFlowMgr.h"

#include "CCameraController.h"

CElfilisD_Roar::CElfilisD_Roar()
    : m_AccTime(0.f)
    , m_bFrmEnter(true)
{
}

CElfilisD_Roar::~CElfilisD_Roar()
{
}

void CElfilisD_Roar::tick()
{
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 140))
    {
        //@EFFECT ¿ÕÇÇ¸®½º BURN IN ÀÌÆåÆ®
        ELFBIGFSM->Activate();
        ELFBIGFSM->ChangeState(L"ROAR");

        // Camera : Ãà¼Ò, ¼ÎÀÌÅ©
        CAMERACTRL->SetLookDist(1000.f);

        CAMERACTRL->SetZoomMinSpeed(0.f);
        CAMERACTRL->SetZoomMaxSpeed(700.f);
        CAMERACTRL->SetZoomThreshold(1000.f);

        m_bFrmEnter = false;
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ELFFSM->SetGlobalState(false);
        ELFFSM->SetPattern(ElfilisPatternType::Appear2);
        ELFFSM->ProcPatternStep();
    }
}

void CElfilisD_Roar::Enter()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("Roar"), false);
    m_bFrmEnter = true;

    ELFFSM->SetGlobalState(true);

    // Camera : È®´ë, ¼ÎÀÌÅ©
    CAMERACTRL->SetLock(false);

    CAMERACTRL->SetLookDist(150.f);

    CAMERACTRL->SetZoomMinSpeed(0.f);
    CAMERACTRL->SetZoomMaxSpeed(500.f);
    CAMERACTRL->SetZoomThreshold(100.f);

    CAMERACTRL->Shake(4.5f, 60.f, 60.f);

    // sound
    wstring Roar = L"sound\\wav\\CharaBossChimera2\\0027_Roar.wav";
    GamePlayStatic::Play2DSound(Roar, 1, SOUND_ELFILIS);

    // Sound
    wstring ResistBGM = L"sound\\stream\\K15_ChimeraSoulCoreStart\\K15_ChimeraSoulCoreStart.wav";
    GamePlayStatic::Play2DSound(ResistBGM, 1, SOUND_BGM);
}

void CElfilisD_Roar::Exit()
{
    CBossMgr::GetBossFlowMgr()->ChangeFlow(BossLevelFlow::Fight);

    // Camera : ÇÏ´Ã ºä
    CAMERACTRL->SetElfilisSky();
}