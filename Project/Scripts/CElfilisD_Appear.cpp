#include "pch.h"
#include "CElfilisD_Appear.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CChangeAlphaScript.h"

#include "CCameraController.h"

CElfilisD_Appear::CElfilisD_Appear()
    : m_StartPos(Vec3(0.f, 800.f, 0.f))
    , m_DownSpeed(200.f)
    , m_AccTime(0.f)
    , m_BossName(nullptr)
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

void CElfilisD_Appear::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("BallWait"), true, false, 1.f, 0.f);
        GetOwner()->Transform()->SetWorldPos(m_StartPos);
        GetOwner()->Transform()->SetWorldRotation(Vec3(0.f, XMConvertToRadians(180.f), 0.f));

        // down
        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, -2000.f, 0.f), ForceMode::Impulse);
        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        m_AccTime = 0.f;

        //@CAMERA : 에피리스 가까이, 등장 바라보며 고정
        CAMERACTRL->SetMainTarget(BOSS);

        CAMERACTRL->FixedView(true, Vec3(0.f, 151.3f, -279.88f));
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 75.f, 0.f));
        CAMERACTRL->SetRotationSpeed(150.f);

        // CAMERACTRL->SetLookDist(130.f);
        // CAMERACTRL->SetLookDir(Vec3(0.f, 0.058f, 0.998f));

        // 설정으로 카메라 즉시이동
        // CAMERACTRL->ResetCamera();
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Appear"), false, false, 1.f, 0.f);

        // spawn BossName
        if (m_BossNamePref != nullptr)
        {
            m_BossName = m_BossNamePref->Instantiate();
            CChangeAlphaScript* Script = m_BossName->GetScript<CChangeAlphaScript>();

            m_BossName->Transform()->SetWorldPos(Vec3(0.f, 905.f, 1570.f));
            m_BossName->Transform()->SetWorldRotation(Vec3(0.f, XMConvertToRadians(180.f), 0.f));
            Script->FadeIn(0.5f);

            GamePlayStatic::SpawnGameObject(m_BossName, LAYER_STATIC);
        }

        //@CAMERA : 뒤로 이동
        // CAMERACTRL->SetTargetOffset(Vec3(0.f, 75.f, 0.f));
        // CAMERACTRL->SetLookDist(280.f);
        // CAMERACTRL->SetLookDir(Vec3(0.f, 0.024f, 0.971f));
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
            Script->FadeOutDestroy(1.f);
            m_BossName = nullptr;
        }

        //@CAMERA : 투타겟
    }
    break;
    }
}

void CElfilisD_Appear::Start()
{
    m_AccTime += DT;
    float AppearTime = 4.f;

    // Add drag
    float CurDist = (GetOwner()->Transform()->GetWorldPos() - m_StartPos).Length();
    float t = CurDist / m_StartPos.Length();
    float Ratio = clamp(t, 0.f, 1.f) * XM_PI;
    float NewDrag = 4.f - 4.f * sinf(Ratio);
    GetOwner()->Rigidbody()->SetDrag(NewDrag);

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