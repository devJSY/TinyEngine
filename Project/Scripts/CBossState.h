#pragma once
#include "CState.h"
#include "CBossMgr.h"
#include "CPlayerMgr.h"

class CBossState : public CState
{
protected:
    StateStep m_Step;

public:
    virtual void tick() override{};
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void ChangeStep(StateStep _Step);
    virtual void Enter_Step() = 0;
    virtual void Exit_Step() = 0;

public:
    CBossState* Clone() = 0;
    CBossState();
    virtual ~CBossState();
};
