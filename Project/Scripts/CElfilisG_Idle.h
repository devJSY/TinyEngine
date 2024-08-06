#pragma once
#include "CBossState.h"

class CElfilisG_Idle : public CBossState
{
private:
    float m_AccTime;

public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

public:
    CLONE(CElfilisG_Idle)
    CElfilisG_Idle();
    virtual ~CElfilisG_Idle();
};