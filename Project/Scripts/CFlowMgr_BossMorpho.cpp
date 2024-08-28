#include "pch.h"
#include "CFlowMgr_BossMorpho.h"
#include "CBossMgr.h"
#include "CMorphoFSM.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"
#include "CCameraController.h"

CFlowMgr_BossMorpho::CFlowMgr_BossMorpho()
    : CLevelFlowMgr(FLOWMGR_BOSSMORPHO)
    , m_FlowState(BossLevelFlow::LevelStart)
    , m_SpawnButterfly(nullptr)
    , m_Barricade(nullptr)
    , m_AccTime(0.f)
{
}

CFlowMgr_BossMorpho::CFlowMgr_BossMorpho(const CFlowMgr_BossMorpho& _Origin)
    : CLevelFlowMgr(_Origin)
    , m_FlowState(BossLevelFlow::LevelStart)
    , m_SpawnButterfly(nullptr)
    , m_Barricade(nullptr)
    , m_BarricadeScale(_Origin.m_BarricadeScale)
    , m_AccTime(0.f)
{
}

CFlowMgr_BossMorpho::~CFlowMgr_BossMorpho()
{
}

void CFlowMgr_BossMorpho::begin()
{
    CLevelFlowMgr::begin();

    // start level
    SetStartLevel(true);
    LevelStart();
    SetEnterTime(6.8f);

    // set effect
    SetToneMappingParam(true, 1, 0.85f, 0.27f, 1.3f, 1.26f, 1.963f);
    SetFadeEffectColor(Vec3(255.f, 150.f, 100.f));

    // find object
    m_SpawnButterfly = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Morpho_SpawnButterfly");
    m_Barricade = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"MorphoBarricade");
    m_BarricadeScale = (m_Barricade) ? m_Barricade->Transform()->GetLocalScale() : Vec3();
}

void CFlowMgr_BossMorpho::tick()
{
    CLevelFlowMgr::tick();

    switch (m_FlowState)
    {
    case BossLevelFlow::LevelStart: {
        BOSS->SetActive(false);

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

        m_FlowState = BossLevelFlow::WaitBoss;
        m_AccTime = 0.f;
    }
    break;

    case BossLevelFlow::WaitBoss:
    {
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
    case BossLevelFlow::Fight:
        break;
    case BossLevelFlow::Death:
        break;
    case BossLevelFlow::Clear:
        break;
    }
}

void CFlowMgr_BossMorpho::LevelStart()
{
    CLevelFlowMgr::LevelStart();

    CGameObject* Camera = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera");
    CCameraController* CameraController = Camera ? Camera->GetScript<CCameraController>() : nullptr;

    if (CameraController)
    {
        Vec3 Dir = Vec3(0.f, -0.26f, -0.97f).Normalize();
        CameraController->SetLookDir(Dir);
        CameraController->SetLookDist(300.f);
        CameraController->SetRotationSpeed(27.5f);
    }
}

void CFlowMgr_BossMorpho::ChangeFlowFight()
{
    if (m_FlowState == BossLevelFlow::WaitBoss)
    {
        TurnOnPlayerHP();
        TurnOnBossHP();
    }

    PLAYERCTRL->UnlockInput();
    
    if (m_Barricade)
    {
        m_Barricade->Transform()->SetWorldScale(m_BarricadeScale);
    }

    m_FlowState = BossLevelFlow::Fight;
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

void CFlowMgr_BossMorpho::ChangeFlowDeath()
{
    TurnOffPlayerHP();
    TurnOffBossHP();

    m_FlowState = BossLevelFlow::Death;
}

void CFlowMgr_BossMorpho::ChangeFlowClear()
{
    MRPFSM->ChangeStateGroup(MorphoStateGroup::Idle, L"IDLE");
    BOSS->SetActive(false);

    CAMERACTRL->SetMainTarget(PLAYER);
    CAMERACTRL->Normal(true);
    CAMERACTRL->SetImmediate(false);
    
    PLAYERFSM->ChangeState(L"STAGE_CLEAR");

    m_FlowState = BossLevelFlow::Clear;
}

void CFlowMgr_BossMorpho::TriggerEvent(UINT _Idx)
{
    if (_Idx == 0)
    {
        SpawnMorpho();
    }
}

void CFlowMgr_BossMorpho::SpawnMorpho()
{
    BOSS->SetActive(true);
    MRPFSM->ChangeStateGroup(MorphoStateGroup::DEMO, L"DEMO_APPEAR");
    PLAYERCTRL->LockInput();

    if (m_SpawnButterfly)
    {
        m_SpawnButterfly->SetActive(false);
    }

    if (m_Barricade)
    {
        m_Barricade->SetActive(true);
    }

    // ø¨√‚ Ω√ PlayerHP TrunOff
    TurnOffPlayerHP();
}

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