#pragma once
#include "CBossState.h"

class CElfilisG_BackStep : public CBossState
{
private:
    float m_PrevDrag;
    float m_NewDrag;

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
