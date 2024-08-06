#include "pch.h"
#include "CElfilisD_Appear.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CChangeAlphaScript.h"

#include "CCameraController.h"

CElfilisD_Appear::CElfilisD_Appear()
    : m_StartPos(Vec3(0.f, 200.f, 0.f))
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
        GetOwner()->Rigidbody()->SetKinematic(true);
        GetOwner()->Transform()->SetWorldPos(m_StartPos);

        m_AccTime = 0.f;

        //@CAMERA : 에피리스 가까이, 등장 바라보며 고정
        CAMERACTRL->SetMainTarget(BOSS);
        CAMERACTRL->SetTargetOffset(Vec3(0.f,75.f,0.f));
        CAMERACTRL->SetLookDist(130.f);
        CAMERACTRL->SetLookDir(Vec3(0.f,0.058f,0.998f));

        // 설정으로 카메라 즉시이동
        CAMERACTRL->ResetCamera();
    }
    break;
    case StateStep::Progress:
    {
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
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 75.f, 0.f));
        CAMERACTRL->SetLookDist(280.f);
        CAMERACTRL->SetLookDir(Vec3(0.f, 0.024f, 0.971f));

    }
        break;
    }
}

void CElfilisD_Appear::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress:
    {
        GetOwner()->Rigidbody()->SetKinematic(false);

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
    bool bFinish = false;
    m_AccTime += DT;

    // move down
    Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
    float CurDist = (NewPos - m_StartPos).Length();
    float Ratio = clamp((CurDist / m_StartPos.Length()), 0.f, 1.f) * XM_PI / 2.f;
    float NewSpeed = 5.f + (m_DownSpeed - 5.f) * cosf(Ratio);

    NewPos.y -= DT * m_DownSpeed;
    if (NewPos.y <= 0.f)
    {
        NewPos.y = 0.f;
        bFinish = true;
    }
    
    GetOwner()->Transform()->SetWorldPos(NewPos);

    if (bFinish)
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