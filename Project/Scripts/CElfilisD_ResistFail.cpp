#include "pch.h"
#include "CElfilisD_ResistFail.h"
#include "CElfilisFSM.h"
#include "CCameraController.h"
#include "CFlowMgr_BossElfilis.h"

CElfilisD_ResistFail::CElfilisD_ResistFail()
    : m_AccTime(0.f)
{
}

CElfilisD_ResistFail::~CElfilisD_ResistFail()
{
}

void CElfilisD_ResistFail::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Progress: {
        Progress();
    }
    break;
    }
}

void CElfilisD_ResistFail::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ResistFail"), false);
        m_AccTime = 0.f;

        // TimeScale & Camera Àá±ñ ¸ØÃã
        CTimeMgr::GetInst()->SetTimeScale(0.5f, 0.f);
        CAMERACTRL->Shake(0.3f, 30.f, 30.f);
        CAMERACTRL->SetLock(true, 0.5f);
    }
    break;
    case StateStep::Progress: {
        m_AccTime = 0.f;

        // Flow Mgr
        CBossMgr::GetElfilisFlowMgr()->ChangeFlowDeath();

        //@CAMERA ¿¡ÇÇ¸®½º Å¸°Ù, ½Ã³×¸¶ºä
        CAMERACTRL->SetMainTarget(BOSS);
        CAMERACTRL->SetOffset(Vec3(0.f, 0.f, 0.f));
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 0.f, 0.f));
        CAMERACTRL->SetSubTargetOffset(Vec3(0.f, 0.f, 0.f));

        Vec3 BossDirFront = BOSS->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 BossDirRight = BOSS->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
        Vec3 CamDir = BossDirFront + BossDirRight;
        CamDir.Normalize();
        CamDir.y = tanf(40.f * XM_PI / 180.f);
        CamDir.Normalize();

        CAMERACTRL->SetLookDir(-CamDir);
        CAMERACTRL->SetLookDist(400.f);
        CAMERACTRL->Normal(true);
    }
    break;
    }
}

void CElfilisD_ResistFail::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress: {
        ELFFSM->ResetEmissive();

        //@CAMERA Ä¿ºñÅ¸°Ù
        CAMERACTRL->SetMainTarget(PLAYER);
        CAMERACTRL->LoadInitSetting();
        CAMERACTRL->ResetCamera();
    }
    break;
    }
}

void CElfilisD_ResistFail::Start()
{
    m_AccTime += DT;

    if (m_AccTime > 0.5f)
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisD_ResistFail::Progress()
{
    m_AccTime += DT;

    // emissive
    float ProgressTime = 2.f;
    float t1 = m_AccTime / ProgressTime;
    float t2 = sinf(t1 * XM_PI / 2.f) * 2.f;
    Vec3 Color = Vec3(t2, t2, t1);

    ELFFSM->AddEmissive(Color);

    if (m_AccTime > 3.f)
    {
        //@EFFECT ÅÍÁö´Â ÆÄÆ¼Å¬
        GetOwner()->SetActive(false);
    }
}