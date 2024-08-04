#pragma once
#include "CBossState.h"

class CElfilisA_Idle : public CBossState
{
public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

public:
    CLONE(CElfilisA_Idle)
    CElfilisA_Idle();
    virtual ~CElfilisA_Idle();
};
