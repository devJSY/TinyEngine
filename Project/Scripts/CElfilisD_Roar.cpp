#include "pch.h"
#include "CElfilisD_Roar.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CElfilisBigFSM.h"
#include "CFlowMgr_BossElfilis.h"

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
        ELFFSM->SetPattern(ElfilisPatternType::Appear2);
        ELFFSM->ProcPatternStep();
    }
}

void CElfilisD_Roar::Enter()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("Roar"), false);
    m_bFrmEnter = true;

    // Camera : È®´ë, ¼ÎÀÌÅ©
    CAMERACTRL->SetLock(false);

    CAMERACTRL->SetLookDist(150.f);

    CAMERACTRL->SetZoomMinSpeed(0.f);
    CAMERACTRL->SetZoomMaxSpeed(500.f);
    CAMERACTRL->SetZoomThreshold(100.f);

    CAMERACTRL->Shake(4.5f, 60.f, 60.f);
}

void CElfilisD_Roar::Exit()
{
    CBossMgr::GetElfilisFlowMgr()->ChangeFlowFight();

    // Camera : ÇÏ´Ã ºä
    CAMERACTRL->SetElfilisSky();
}