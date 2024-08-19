#include "pch.h"
#include "CElfilisD_Appear.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CChangeAlphaScript.h"
#include "CFlowMgr_BossElfilis.h"

#include "CCameraController.h"

CElfilisD_Appear::CElfilisD_Appear()
    : m_BossName(nullptr)
    , m_StartPos(Vec3(0.f, 800.f, 0.f))
    , m_PrevDrag(0.f)
    , m_DownSpeed(200.f)
    , m_AccTime(0.f)
    , m_bFrmEnter(false)
{
    m_BossNamePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\BossName_Elfilis.pref", L"prefab\\BossName_Elfilis.pref");
}

CElfilisD_Appear::~CElfilisD_Appear()
{
}

void CElfilisD_Appear::tick()
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

void CElfilisD_Appear::Exit()
{
    Exit_Step();

    if (CBossMgr::GetElfilisFlowMgr())
    {
        CBossMgr::GetElfilisFlowMgr()->SetFight();
    }
}

void CElfilisD_Appear::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("BallWait"), true, false, 1.f, 0.f);
        GetOwner()->Transform()->SetWorldPos(m_StartPos);
        GetOwner()->Transform()->SetWorldRotation(Vec3(0.f, 0.f, 0.f));
        m_bFrmEnter = true;

        // 에피리스 가까이, 등장 바라보며 고정
        CAMERACTRL->SetMainTarget(BOSS);

        CAMERACTRL->FixedView(true, Vec3(0.f, 83.65f, 147.6f));

        CAMERACTRL->SetOffset(Vec3(0.f,0.f,0.f));
        CAMERACTRL->SetMinSpeed(200.f);
        CAMERACTRL->SetMaxSpeed(500.f);
        CAMERACTRL->SetThresholdDistance(50.f);
        CAMERACTRL->SetRotationSpeed(150.f);
        CAMERACTRL->SetZoomMinSpeed(50.f);
        CAMERACTRL->SetZoomMaxSpeed(360.f);
        CAMERACTRL->SetZoomThreshold(50.f);    
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 75.f, 0.f));

        // 설정으로 카메라 즉시이동
        CAMERACTRL->ResetCamera();
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Appear"), false, false, 1.f, 0.f);

        // spawn BossName
        if (m_BossNamePref != nullptr)
        {
            m_BossName = m_BossNamePref->Instantiate();
            CChangeAlphaScript* Script = m_BossName->GetScript<CChangeAlphaScript>();

            m_BossName->Transform()->SetWorldPos(Vec3(0.f, 905.f, -2200.f));
            m_BossName->Transform()->SetWorldRotation(Vec3(0.f, 0.f, 0.f));
            Script->FadeIn(0.5f);

            GamePlayStatic::SpawnGameObject(m_BossName, LAYER_STATIC);
        }

        // 뒤로 이동
        CAMERACTRL->Normal(false);
        CAMERACTRL->SetLookDist(300.f);
        CAMERACTRL->SetLookDir(Vec3(0.f, 0.024f, -0.971f));

        CAMERACTRL->SetOffset(Vec3(0.f, 0.f, 0.f));
        CAMERACTRL->SetMinSpeed(0.f);
        CAMERACTRL->SetMaxSpeed(100.f);
        CAMERACTRL->SetThresholdDistance(10.f);
        CAMERACTRL->SetRotationSpeed(30.f);
        CAMERACTRL->SetZoomMinSpeed(0.f);
        CAMERACTRL->SetZoomMaxSpeed(50.f);
        CAMERACTRL->SetZoomThreshold(50.f);
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 75.f, 0.f));
    }
    break;
    }
}

void CElfilisD_Appear::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
    }
    break;
    case StateStep::Progress: {
        if (m_BossName)
        {
            CChangeAlphaScript* Script = m_BossName->GetScript<CChangeAlphaScript>();
            Script->FadeOutDestroy(0.5f);
            m_BossName = nullptr;
        }

        // 투타겟
        CAMERACTRL->LoadInitSetting();

        CAMERACTRL->SetElfilisTwoTarget();
    }
    break;
    }
}

void CElfilisD_Appear::Start()
{
    m_AccTime += DT;
    float AppearTime = 4.f;

    // down
    if (m_bFrmEnter)
    {
        m_bFrmEnter = false;

        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, -2000.f, 0.f), ForceMode::Impulse);
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        m_AccTime = 0.f;
    }

    // Add drag
    else
    {
        float CurDist = (GetOwner()->Transform()->GetWorldPos() - m_StartPos).Length();
        float t = CurDist / m_StartPos.Length();
        float Ratio = clamp(t, 0.f, 1.f) * XM_PI;
        float NewDrag = 4.f - 4.f * sinf(Ratio);
        GetOwner()->Rigidbody()->SetDrag(NewDrag);
    }

    if (m_AccTime > AppearTime)
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisD_Appear::Progress()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ELFFSM->SetPattern(ElfilisPatternType::Appear1);
        ELFFSM->ProcPatternStep();
    }
}