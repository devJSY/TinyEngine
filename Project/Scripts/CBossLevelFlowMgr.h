#pragma once
#include "CLevelFlowMgr.h"
#include "CBossMgr.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"
#include "CCameraController.h"

class CBossLevelFlowMgr : public CLevelFlowMgr
{
protected:
    BossLevelFlow m_FlowState;
    Vec3 m_DefaultDemoPos;

public:
    virtual void begin() override;
    virtual void tick() override;

    void ChangeFlow(BossLevelFlow _State);

protected:
    void EnterFlow(BossLevelFlow _State);
    void ExitFlow(BossLevelFlow _State);
    void SetPlayerPos(Vec3 _Pos);

    // tick
    virtual void FlowLevelStart();
    virtual void FlowWaitBoss(){};
    virtual void FlowFight(){};
    virtual void FlowDemoPlay(){};
    virtual void FlowDeath(){};
    virtual void FlowClear(){};

    // flow enter
    virtual void EnterLevelStart(){};
    virtual void EnterWaitBoss(){};
    virtual void EnterFight();
    virtual void EnterDemoPlay();
    virtual void EnterDeath();
    virtual void EnterClear();

    // flow exit
    virtual void ExitLevelStart(){};
    virtual void ExitWaitBoss();
    virtual void ExitFight(){};
    virtual void ExitDemoPlay();
    virtual void ExitDeath(){};
    virtual void ExitClear(){};

public:
    BossLevelFlow GetFlowState() { return m_FlowState; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CBossLevelFlowMgr);
    CBossLevelFlowMgr(UINT _Type);
    CBossLevelFlowMgr(const CBossLevelFlowMgr& _Origin);
    virtual ~CBossLevelFlowMgr();
};