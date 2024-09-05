#include "pch.h"
#include "CElfilisD_Appear.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CChangeAlphaScript.h"
#include "CBossLevelFlowMgr.h"

#include "CCameraController.h"

CElfilisD_Appear::CElfilisD_Appear()
    : m_BossName(nullptr)
    , m_StartPos(Vec3(0.f, 0.f, 0.f))
    , m_DownSpeed(200.f)
    , m_AccTime(0.f)
    , m_bFrmEnter(false)
    , m_SoundIdx(0)
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

    CBossMgr::GetBossFlowMgr()->ChangeFlow(BossLevelFlow::Fight);
}

void CElfilisD_Appear::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DemoAppearSoulCut2"), false, false, 1.f, 0.f);
        GetOwner()->Transform()->SetWorldPos(m_StartPos);
        GetOwner()->Transform()->SetWorldRotation(Vec3(0.f, -XM_PI/2.f, 0.f));
        m_bFrmEnter = true;

        // Camera : 에피리스가 등장하는 하늘을 올려다봄 (고정)
        CAMERACTRL->SetMainTarget(BOSS);
        CAMERACTRL->FixedView(true, Vec3(0.f, 83.65f, 147.6f));

        CAMERACTRL->SetTargetOffset(Vec3(0.f, 300.f, 0.f));
        CAMERACTRL->SetMinSpeed(200.f);
        CAMERACTRL->SetMaxSpeed(500.f);
        CAMERACTRL->SetThresholdDistance(50.f);
        CAMERACTRL->SetRotationSpeed(150.f);
        CAMERACTRL->SetZoomMinSpeed(50.f);
        CAMERACTRL->SetZoomMaxSpeed(360.f);
        CAMERACTRL->SetZoomThreshold(50.f);

        CAMERACTRL->ResetCamera();
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DemoAppearSoulCut3"), false, false, 2.5f, 0.f);
        m_bFrmEnter = true;
        m_SoundIdx = 0;

        // Camera : 에피리스를 정면으로 바라봄 (고정)
        CAMERACTRL->FixedView(true, Vec3(0.f, 50.f, 55.f));

        CAMERACTRL->SetOffset(Vec3(0.f, 0.f, 0.f));
        CAMERACTRL->SetMinSpeed(200.f);
        CAMERACTRL->SetMaxSpeed(500.f);
        CAMERACTRL->SetThresholdDistance(50.f);
        CAMERACTRL->SetRotationSpeed(150.f);
        CAMERACTRL->SetZoomMinSpeed(50.f);
        CAMERACTRL->SetZoomMaxSpeed(360.f);
        CAMERACTRL->SetZoomThreshold(50.f);
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 65.f, 0.f));

        CAMERACTRL->SaveSetting();
        CAMERACTRL->ResetCamera();
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
    }
    break;
    case StateStep::Progress: {
        if (m_BossName)
        {   
            CChangeAlphaScript* Script = m_BossName->GetScript<CChangeAlphaScript>();
            Script->FadeOutDestroy(0.5f);
            m_BossName = nullptr;
        }

        // Camera : 투타겟
        CAMERACTRL->LoadInitSetting(true);
        CAMERACTRL->SetElfilisTwoTarget();
    }
    break;
    }
}

void CElfilisD_Appear::Start()
{
     // Sound
    if (CHECK_ANIMFRM(GetOwner(), 60) && m_bFrmEnter)
    {
        m_bFrmEnter = false;

        // Sound
        wstring BossDown = L"sound\\wav\\DemoChimeraSoulPre\\0000_BossDown.wav";
        GamePlayStatic::Play2DSound(BossDown, 1, SOUND_ELFILIS);
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisD_Appear::Progress()
{
    if (CHECK_ANIMFRM(GetOwner(), 330) && m_bFrmEnter)
    {
        m_bFrmEnter = false;

        // spawn BossName
        if (m_BossNamePref != nullptr)
        {
            m_BossName = m_BossNamePref->Instantiate();
            CChangeAlphaScript* Script = m_BossName->GetScript<CChangeAlphaScript>();

            m_BossName->Transform()->SetWorldPos(Vec3(0.f, 3000.f, -4020.f));
            m_BossName->Transform()->SetWorldRotation(Vec3(0.f, 0.f, 0.f));
            m_BossName->Transform()->SetWorldScale(Vec3(427.f));
            Script->FadeIn(0.5f);

            GamePlayStatic::SpawnGameObject(m_BossName, LAYER_STATIC);
        }

        // Camera : 뒤로 이동
        CAMERACTRL->Normal(false);
        CAMERACTRL->SetLookDist(200.f);
        CAMERACTRL->SetLookDir(Vec3(0.f, 0.315f, -0.949f));

        CAMERACTRL->SetOffset(Vec3(0.f, 0.f, 0.f));
        CAMERACTRL->SetMinSpeed(0.f);
        CAMERACTRL->SetMaxSpeed(40.f);
        CAMERACTRL->SetThresholdDistance(10.f);
        CAMERACTRL->SetRotationSpeed(10.f);
        CAMERACTRL->SetZoomMinSpeed(0.f);
        CAMERACTRL->SetZoomMaxSpeed(20.f);
        CAMERACTRL->SetZoomThreshold(50.f);
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 90.f, 0.f));
    }

    // Sound
    if (m_SoundIdx == 0 && CHECK_ANIMFRM(GetOwner(), 270))
    {
        m_bFrmEnter = true;
        m_SoundIdx++;

        wstring BossAppear = L"sound\\wav\\DemoChimeraSoulPre\\0001_BossAppear.wav";
        GamePlayStatic::Play2DSound(BossAppear, 1, SOUND_ELFILIS);
    }
    else if (m_SoundIdx == 1 && CHECK_ANIMFRM(GetOwner(), 550))
    {
        m_bFrmEnter = false;
        m_SoundIdx++;

        wstring SpawnHalberd = L"sound\\wav\\DemoChimeraSoulPre\\0002_SpawnHalberd.wav";
        GamePlayStatic::Play2DSound(SpawnHalberd, 1, SOUND_ELFILIS * 0.5f);
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ELFFSM->SetPattern(ElfilisPatternType::Appear1);
        ELFFSM->ProcPatternStep();
    }
}