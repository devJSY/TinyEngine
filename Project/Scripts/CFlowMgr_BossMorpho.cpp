#include "pch.h"
#include "CFlowMgr_BossMorpho.h"
#include "CMorphoFSM.h"

CFlowMgr_BossMorpho::CFlowMgr_BossMorpho()
    : CBossLevelFlowMgr(FLOWMGR_BOSSMORPHO)
    , m_SpawnButterfly(nullptr)
    , m_Barricade(nullptr)
    , m_AccTime(0.f)
    , m_EventIdx(0)
{
    m_DefaultDemoPos = Vec3(0.f, 0.f, 300.f);
}

CFlowMgr_BossMorpho::CFlowMgr_BossMorpho(const CFlowMgr_BossMorpho& _Origin)
    : CBossLevelFlowMgr(_Origin)
    , m_SpawnButterfly(nullptr)
    , m_Barricade(nullptr)
    , m_BarricadeScale(_Origin.m_BarricadeScale)
    , m_AccTime(0.f)
    , m_EventIdx(0)
{
    m_DefaultDemoPos = Vec3(0.f, 0.f, 300.f);
}

CFlowMgr_BossMorpho::~CFlowMgr_BossMorpho()
{
}

void CFlowMgr_BossMorpho::begin()
{
    CBossLevelFlowMgr::begin();

    // set effect
    SetToneMappingParam(true, 1, 0.4f, 0.27f, 1.3f, 1.6f, 2.3f);
    SetFadeEffectColor(Vec3(255.f, 150.f, 100.f));

    // find object
    m_SpawnButterfly = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Morpho_SpawnButterfly");
    m_Barricade = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"MorphoBarricade");
    m_BarricadeScale = (m_Barricade) ? m_Barricade->Transform()->GetLocalScale() : Vec3();
}

void CFlowMgr_BossMorpho::LevelStart()
{
    CLevelFlowMgr::LevelStart();
    FadeInBGM(L"sound\\stream\\K15_MorphoKnight1\\K15_MorphoKnight1.marker.wav", 0.f, SOUND_BGM, 1.f);
}

void CFlowMgr_BossMorpho::LevelEnd()
{
    CLevelFlowMgr::LevelEnd();
    SetFadeEffectColor(Vec3(164.f, 44.f, 174.f));
}

void CFlowMgr_BossMorpho::LevelRestart()
{
    CLevelFlowMgr::LevelRestart();
    SetFadeEffectColor(Vec3(255.f, 150.f, 100.f));
}

void CFlowMgr_BossMorpho::TriggerEvent(UINT _Idx)
{
    if (_Idx == 0)
    {
        DeleteBubble();
        SpawnMorpho();
    }
}

// ---------------------------
// Overrided Flow Events
// ---------------------------
void CFlowMgr_BossMorpho::FlowLevelStart()
{
    if (m_SpawnButterfly)
    {
        m_SpawnButterfly->GetChildObject(L"ButterflyTriple0")->Animator()->Play(ANIMPREFIX("Wait"), true, false, 1.f);
        m_SpawnButterfly->GetChildObject(L"ButterflyTriple1")->Animator()->Play(ANIMPREFIX("Wait"), true, false, 1.f);
    }

    if (m_Barricade)
    {
        m_Barricade->SetActive(false);
        m_Barricade->Transform()->SetLocalScale(Vec3::Zero);
    }

    // set camera
    CGameObject* Camera = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera");
    CCameraController* CameraController = Camera ? Camera->GetScript<CCameraController>() : nullptr;

    if (CameraController)
    {
        Vec3 Dir = Vec3(0.f, -0.26f, -0.97f).Normalize();
        CameraController->SetLookDir(Dir);
        CameraController->SetLookDist(300.f);
        CameraController->SetRotationSpeed(27.5f);
    }

    CBossLevelFlowMgr::FlowLevelStart();
}

void CFlowMgr_BossMorpho::EnterDemoPlay()
{
    CBossLevelFlowMgr::EnterDemoPlay();
    m_AccTime = 0.f;

    switch (m_DemoType)
    {
    case BossDemoType::Encounter: {
        // SetToneMappingParam(true, 1, 0.5f, 0.27f, 1.3f, 1.2f, 1.7f);
        // SetToneMappingParam(true, 1, 0.5f, 0.27f, 1.3f, 1.26f, 1.973f);
    }
    break;
    case BossDemoType::StartPhase2:
        break;
    }
}

void CFlowMgr_BossMorpho::FlowDemoPlay()
{
    CBossLevelFlowMgr::FlowDemoPlay();

    switch (m_DemoType)
    {
    case BossDemoType::Encounter: {
        float SpawnBarricadeTime = 3.f;

        if (m_Barricade && m_Barricade->IsActive() && m_AccTime < SpawnBarricadeTime)
        {
            m_AccTime += DT;

            float t = m_AccTime / SpawnBarricadeTime;
            Vec3 NewScale = m_BarricadeScale;
            NewScale.y *= t;

            m_Barricade->Transform()->SetWorldScale(NewScale);
        }
    }
    break;
    case BossDemoType::StartPhase2: {
        float t = BOSS->Animator()->IsChainging() ? 0.f : BOSS->Animator()->GetClipPlayRatio();
        float BlurStart = 0.5f;

        if (t >= BlurStart)
        {
            t = (t - BlurStart) / (1.f - BlurStart);
            float BlurPow = 5.f * sinf(XM_PI * t);
            float BlurRadius = 10.f + clamp(sinf(XM_PI * t) * 1.5f, 0.f, 1.f);

            SetRadialBlurEffect(BlurRadius, BlurPow, Vec2(0.5f));
            GetRadialBlurEffect()->SetActive(true);
        }
    }
    break;
    }
}

void CFlowMgr_BossMorpho::ExitDemoPlay()
{
    switch (m_DemoType)
    {
    case BossDemoType::Encounter: {
        float SpawnBarricadeTime = 3.f;

        if (m_Barricade && m_AccTime >= SpawnBarricadeTime)
        {
            m_Barricade->Transform()->SetWorldScale(m_BarricadeScale);
        }

        // SetToneMappingParam(true, 1, 0.85f, 0.27f, 1.3f, 1.26f, 1.963f);
    }
    break;
    case BossDemoType::StartPhase2: {
        GetRadialBlurEffect()->SetActive(false);
    }
    break;
    }

    CBossLevelFlowMgr::ExitDemoPlay();
}

void CFlowMgr_BossMorpho::EnterFight()
{
    CBossLevelFlowMgr::EnterFight();

    if (m_Barricade)
    {
        m_Barricade->Transform()->SetWorldScale(m_BarricadeScale);
    }
}

void CFlowMgr_BossMorpho::EnterDeath()
{
    CBossLevelFlowMgr::EnterDeath();

    SetPlayerPos(m_DefaultDemoPos, -m_DefaultDemoPos);
    PLAYERCTRL->LockInput();
}

void CFlowMgr_BossMorpho::ExitDeath()
{
    CBossLevelFlowMgr::ExitDeath();

    PLAYERCTRL->UnlockInput();
}

void CFlowMgr_BossMorpho::EnterClear()
{
    // CBossLevelFlowMgr::EnterClear();

    BOSS->SetActive(false);
    MRPFSM->ChangeStateGroup(MorphoStateGroup::Idle, L"IDLE");

    SetPlayerPos(Vec3(), Vec3(0.f, 0.f, 1.f));
    PLAYERCTRL->LockInput();

    m_AccTime = 0.f;
    m_EventIdx = 0;
}

void CFlowMgr_BossMorpho::FlowClear()
{
    CBossLevelFlowMgr::FlowClear();
    m_AccTime += DT;

    if (m_EventIdx == 0 && m_AccTime > 2.f)
    {
        m_EventIdx++;

        CAMERACTRL->SetLock(false);
        CAMERACTRL->SetMainTarget(PLAYER);
        CAMERACTRL->SetImmediate(false);

        PLAYERFSM->ChangeState(L"STAGE_CLEAR");
    }
}

// ---------------------------
// Trigger Events
// ---------------------------
void CFlowMgr_BossMorpho::SpawnMorpho()
{
    BOSS->SetActive(true);
    MRPFSM->ChangeStateGroup(MorphoStateGroup::DEMO, L"DEMO_APPEAR");

    if (m_SpawnButterfly)
    {
        m_SpawnButterfly->SetActive(false);
    }

    if (m_Barricade)
    {
        m_Barricade->SetActive(true);
    }

    SetDemoType(BossDemoType::Encounter);
    ChangeFlow(BossLevelFlow::DemoPlay);
}

// ---------------------------
// Save & Load
// ---------------------------
UINT CFlowMgr_BossMorpho::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_BossMorpho::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}