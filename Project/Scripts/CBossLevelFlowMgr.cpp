#include "pch.h"
#include "CBossLevelFlowMgr.h"

CBossLevelFlowMgr::CBossLevelFlowMgr(UINT _Type)
    : CLevelFlowMgr(_Type)
    , m_FlowState(BossLevelFlow::LevelStart)
    , m_DemoType(BossDemoType::NONE)
{
}

CBossLevelFlowMgr::CBossLevelFlowMgr(const CBossLevelFlowMgr& _Origin)
    : CLevelFlowMgr(_Origin)
    , m_FlowState(BossLevelFlow::LevelStart)
    , m_DemoType(BossDemoType::NONE)
    , m_DefaultDemoPos(_Origin.m_DefaultDemoPos)
{
}

CBossLevelFlowMgr::~CBossLevelFlowMgr()
{
}

void CBossLevelFlowMgr::begin()
{
    CLevelFlowMgr::begin();

    // start level
    SetStartLevel(true);
    LevelStart();
    SetEnterTime(6.8f);
}

void CBossLevelFlowMgr::tick()
{
    CLevelFlowMgr::tick();

    switch (m_FlowState)
    {
    case BossLevelFlow::LevelStart:
        FlowLevelStart();
        break;
    case BossLevelFlow::WaitBoss:
        FlowWaitBoss();
        break;
    case BossLevelFlow::Fight:
        FlowFight();
        break;
    case BossLevelFlow::DemoPlay:
        FlowDemoPlay();
        break;
    case BossLevelFlow::Death:
        FlowDeath();
        break;
    case BossLevelFlow::Clear:
        FlowClear();
        break;
    }
}

void CBossLevelFlowMgr::ChangeFlow(BossLevelFlow _State)
{
    ExitFlow(m_FlowState);
    m_FlowState = _State;
    EnterFlow(_State);
}

void CBossLevelFlowMgr::EnterFlow(BossLevelFlow _State)
{
    switch (_State)
    {
    case BossLevelFlow::LevelStart:
        EnterLevelStart();
        break;
    case BossLevelFlow::WaitBoss:
        EnterWaitBoss();
        break;
    case BossLevelFlow::Fight:
        EnterFight();
        break;
    case BossLevelFlow::DemoPlay:
        EnterDemoPlay();
        break;
    case BossLevelFlow::Death:
        EnterDeath();
        break;
    case BossLevelFlow::Clear:
        EnterClear();
        break;
    }
}

void CBossLevelFlowMgr::ExitFlow(BossLevelFlow _State)
{
    switch (_State)
    {
    case BossLevelFlow::LevelStart:
        ExitLevelStart();
        break;
    case BossLevelFlow::WaitBoss:
        ExitWaitBoss();
        break;
    case BossLevelFlow::Fight:
        ExitFight();
        break;
    case BossLevelFlow::DemoPlay:
        ExitDemoPlay();
        break;
    case BossLevelFlow::Death:
        ExitDeath();
        break;
    case BossLevelFlow::Clear:
        ExitClear();
        break;
    }
}

void CBossLevelFlowMgr::SetPlayerPos(Vec3 _Pos, Vec3 _Rot)
{
    _Rot.Normalize();

    PLAYER->Transform()->SetWorldPos(_Pos);
    PLAYERCTRL->ForceDir({ForceDirType::STAGEEVENT, _Rot, true});
}

// --------------------
// Play Flow
// --------------------
void CBossLevelFlowMgr::FlowLevelStart()
{
    BOSS->SetActive(false);

    ChangeFlow(BossLevelFlow::WaitBoss);
}

// --------------------
// Enter Flow
// --------------------
void CBossLevelFlowMgr::EnterFight()
{
    TurnOnPlayerHP();
    TurnOnBossHP();

    PLAYERCTRL->UnlockInput();
}

void CBossLevelFlowMgr::EnterDemoPlay()
{
    SetPlayerPos(m_DefaultDemoPos, -m_DefaultDemoPos);
    PLAYERCTRL->LockInput();
    PLAYERFSM->ChangeState(L"IDLE");
}

void CBossLevelFlowMgr::EnterDeath()
{
    TurnOffPlayerHP();
    TurnOffBossHP();
}

void CBossLevelFlowMgr::EnterClear()
{
    BOSS->SetActive(false);

    PLAYERCTRL->LockInput();
    
    CAMERACTRL->SetMainTarget(PLAYER);
    CAMERACTRL->SetImmediate(false);
    PLAYERFSM->ChangeState(L"STAGE_CLEAR");
}

// --------------------
// Exit Flow
// --------------------
void CBossLevelFlowMgr::ExitWaitBoss()
{
    BOSS->SetActive(true);

    TurnOnPlayerHP();
    TurnOnBossHP();
}

void CBossLevelFlowMgr::ExitDemoPlay()
{
    PLAYERCTRL->UnlockInput();
    m_DemoType = BossDemoType::NONE;
}

void CBossLevelFlowMgr::ExitClear()
{
    PLAYERCTRL->UnlockInput();
}

// --------------------
// Save & Load
// --------------------
UINT CBossLevelFlowMgr::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    MemoryByte += CLevelFlowMgr::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CBossLevelFlowMgr::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    MemoryByte += CLevelFlowMgr::LoadFromLevelFile(_File);

    return MemoryByte;
}