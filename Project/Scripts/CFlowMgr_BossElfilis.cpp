#include "pch.h"
#include "CFlowMgr_BossElfilis.h"
#include "CElfilisFSM.h"

CFlowMgr_BossElfilis::CFlowMgr_BossElfilis()
    : CBossLevelFlowMgr(FLOWMGR_BOSSELFILIS)
    , m_LevelEnterWall(nullptr)
{
    m_DefaultDemoPos = Vec3(0.f, 0.f, 450.f);
}

CFlowMgr_BossElfilis::CFlowMgr_BossElfilis(const CFlowMgr_BossElfilis& _Origin)
    : CBossLevelFlowMgr(_Origin)
    , m_LevelEnterWall(nullptr)
{
    m_DefaultDemoPos = Vec3(0.f, 0.f, 450.f);
}

CFlowMgr_BossElfilis::~CFlowMgr_BossElfilis()
{
}

void CFlowMgr_BossElfilis::begin()
{
    CBossLevelFlowMgr::begin();

    // set effect
    SetToneMappingParam(true, false, 0.4f, 0.75f, 0.3f, 1.2f, 2.2f);
    SetFadeEffectColor(Vec3(180.f, 140.f, 200.f));

    // set camera
    CGameObject* Camera = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera");
    CCameraController* CameraController = Camera ? Camera->GetScript<CCameraController>() : nullptr;

    if (CameraController)
    {
        Vec3 Dir = Vec3(-0.57f, -0.49f, -0.66f).Normalize();
        CameraController->SetLookDir(Dir);
        CameraController->SetLookDist(400.f);
    }

    // find object
    m_LevelEnterWall = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"LevelEnterWall");
}

void CFlowMgr_BossElfilis::TriggerEvent(UINT _Idx)
{
    // Spawn Elfilis Trigger
    if (_Idx == 0)
    {
        SpawnElfilis();
    }
}

void CFlowMgr_BossElfilis::LevelStart()
{
    CBossLevelFlowMgr::LevelStart();

    FadeInBGM(L"sound\\stream\\K15_BossChimeraPerfect_Stest\\K15_BossChimeraPerfect_Stest.marker.wav", 0.f, SOUND_BGM, 1.f);
}

void CFlowMgr_BossElfilis::LevelEnd()
{
    CBossLevelFlowMgr::LevelEnd();
    SetFadeEffectColor(Vec3(252.f, 75.f, 129.f));
}

void CFlowMgr_BossElfilis::LevelRestart()
{
    CBossLevelFlowMgr::LevelRestart();
    SetFadeEffectColor(Vec3(180.f, 140.f, 200.f));
}

// ---------------------------
// Overrided Flow Events
// ---------------------------
void CFlowMgr_BossElfilis::FlowLevelStart()
{
    BOSS->SetActive(false);
    ELFFSM->GetBigBoss()->SetActive(false);

    if (m_LevelEnterWall)
    {
        m_LevelEnterWall->SetActive(false);
    }

    ChangeFlow(BossLevelFlow::WaitBoss);
}

void CFlowMgr_BossElfilis::EnterClear()
{
    ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundIdle, L"GROUND_IDLE");

    SetPlayerPos(ELFFSM->GetMapFloorOffset(), Vec3(0.f, 0.f, -1.f));

    CBossLevelFlowMgr::EnterClear();
}

// ---------------------------
// Trigger Events
// ---------------------------
void CFlowMgr_BossElfilis::SpawnElfilis()
{
    BOSS->SetActive(true);
    ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_APPEAR1");

    if (m_LevelEnterWall)
    {
        m_LevelEnterWall->SetActive(true);
    }

    ChangeFlow(BossLevelFlow::DemoPlay);
}

// ---------------------------
// Save & Load
// ---------------------------
UINT CFlowMgr_BossElfilis::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CBossLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_BossElfilis::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CBossLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}