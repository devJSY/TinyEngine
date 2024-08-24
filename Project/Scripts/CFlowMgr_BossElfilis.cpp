#include "pch.h"
#include "CFlowMgr_BossElfilis.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CCameraController.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"

CFlowMgr_BossElfilis::CFlowMgr_BossElfilis()
    : CLevelFlowMgr(FLOWMGR_BOSSELFILIS)
    , m_FlowState(BossLevelFlow::LevelStart)
    , m_LevelEnterWall(nullptr)
{
}

CFlowMgr_BossElfilis::CFlowMgr_BossElfilis(const CFlowMgr_BossElfilis& _Origin)
    : CLevelFlowMgr(_Origin)
    , m_FlowState(BossLevelFlow::LevelStart)
    , m_LevelEnterWall(nullptr)
{
}

CFlowMgr_BossElfilis::~CFlowMgr_BossElfilis()
{
}

void CFlowMgr_BossElfilis::begin()
{
    CLevelFlowMgr::begin();

    // start level
    SetStartLevel(true);
    LevelStart();
    SetEnterTime(6.8f);

    // set effect
    SetToneMappingParam(true, true, 1.f, 1.88f, 0.3f);
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

void CFlowMgr_BossElfilis::tick()
{
    CLevelFlowMgr::tick();

    switch (m_FlowState)
    {
    case BossLevelFlow::LevelStart: {
        BOSS->SetActive(false);
        ELFFSM->GetBigBoss()->SetActive(false);

        if (m_LevelEnterWall)
        {
            m_LevelEnterWall->SetActive(false);
        }

        m_FlowState = BossLevelFlow::WaitBoss;
    }
    break;
    case BossLevelFlow::WaitBoss:
        break;
    case BossLevelFlow::Fight:
        break;
    case BossLevelFlow::DemoPlay:
        break;
    case BossLevelFlow::Death:
        break;
    }
}

void CFlowMgr_BossElfilis::TriggerEvent(UINT _Idx)
{
    if (_Idx == 0)
    {
        SpawnElfilis();
    }
}

void CFlowMgr_BossElfilis::ChangeFlowFight()
{
    if (m_FlowState == BossLevelFlow::WaitBoss)
    {
        TurnOnPlayerHP();
        TurnOnBossHP();
    }

    PLAYERCTRL->UnlockInput();
    m_FlowState = BossLevelFlow::Fight;
}

void CFlowMgr_BossElfilis::ChangeFlowResist()
{
    Vec3 NewPos{0.f, 0.f, 450.f};
    PLAYER->Transform()->SetWorldPos(NewPos);
    PLAYER->Transform()->Slerp(-NewPos.Normalize(), 1.f);
    PLAYERFSM->ChangeState(L"IDLE");
    PLAYERCTRL->LockInput();

    m_FlowState = BossLevelFlow::DemoPlay;
}

void CFlowMgr_BossElfilis::ChangeFlowDeath()
{
    TurnOffPlayerHP();
    TurnOffBossHP();

    m_FlowState = BossLevelFlow::Death;
}

void CFlowMgr_BossElfilis::LevelEnd()
{
    CLevelFlowMgr::LevelEnd();
    SetFadeEffectColor(Vec3(252.f, 75.f, 129.f));
}

void CFlowMgr_BossElfilis::LevelRestart()
{
    CLevelFlowMgr::LevelRestart();
    SetFadeEffectColor(Vec3(180.f, 140.f, 200.f));
}

void CFlowMgr_BossElfilis::SpawnElfilis()
{
    PLAYERCTRL->LockInput();
    BOSS->SetActive(true);
    ELFFSM->ChangeStateGroup(ElfilisStateGroup::DEMO, L"DEMO_APPEAR1");

    TurnOffPlayerHP();

    if (m_LevelEnterWall)
    {
        m_LevelEnterWall->SetActive(true);
    }
}

UINT CFlowMgr_BossElfilis::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CFlowMgr_BossElfilis::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}