#pragma once
#include "CBossState.h"

class CElfilisG_BackStep : public CBossState
{
private:
    StateStep m_Step;

public:
    virtual void tick();

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void Progress();
    void End();

public:
    CLONE(CElfilisG_BackStep)
    CElfilisG_BackStep();
    virtual ~CElfilisG_BackStep();
};
