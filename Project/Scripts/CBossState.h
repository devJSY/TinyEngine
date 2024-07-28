#pragma once
#include "CState.h"
#include "CBossMgr.h"
#include "CPlayerMgr.h"
#include "CUnitScript.h"

class CBossState : public CState
{
protected:
    StateStep m_Step;

public:
    virtual void tick() override{};
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    virtual void Enter_Step() = 0;
    virtual void Exit_Step() = 0;

    void ChangeStep(StateStep _Step);
    void RotateToPlayer(float _delta = 0.f);

public:
    CBossState* Clone() = 0;
    CBossState();
    virtual ~CBossState();
};
